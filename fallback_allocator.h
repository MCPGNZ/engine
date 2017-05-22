#pragma once
#include "config.h"
#include "memory_block.h"

namespace pk
{
    template <typename Main, typename Fallback>
    class fallback_allocator
    {
    public:
        #pragma region Public methods
        memory_block allocate(const size_type n);
        void deallocate(const memory_block& block);

        bool owns(const memory_block& block) const;
        #pragma endregion

    private:
        #pragma region Private variables
        Main main;
        Fallback fallback;
        #pragma endregion
    };

    #pragma region Public templates
    template <typename Main, typename Fallback>
    memory_block fallback_allocator<Main, Fallback>::allocate(const size_type n)
    {
        auto result = main.allocate(n);
        if(result.is_valid())
        {
            return result;
        }

        return fallback.allocate(n);
    }

    template <typename Main, typename Fallback>
    void fallback_allocator<Main, Fallback>::deallocate(const memory_block& block)
    {
        if(main.owns(block))
        {
            main.deallocate(block);
            return;
        }

        fallback.deallocate(block);
    }

    template <typename Main, typename Fallback>
    bool fallback_allocator<Main, Fallback>::owns(const memory_block& block) const
    {
        return main.owns(block) || fallback.owns(block);
    }
    #pragma endregion
}
