#pragma once
#include "config.h"
#include "memory_block.h"

namespace pk
{
    template <size_type s>
    class stack_allocator
    {
    public:
        #pragma region Ctors
        stack_allocator(void) : pointer{data} {};
        #pragma endregion

        #pragma region Public methods
        memory_block allocate(const size_type n)
        {
            if(!has_capacity(n))
            {
                return {nullptr, 0};
            }

            memory_block result = {pointer, n};
            pointer += n;

            return result;
        }

        void deallocate(const memory_block& m)
        {
            if((reinterpret_cast<byte*>(m.pointer) + m.size) == pointer)
            {
                pointer = reinterpret_cast<byte*>(m.pointer);
            }
        }
        void deallocate_all()
        {
            pointer = data;
        }

        bool owns(const memory_block& m) const
        {
            return (m.pointer >= data) && (m.pointer < (data + s));
        }
        #pragma endregion

    private:
        #pragma region Private variables
        byte data[s];
        memory_address pointer;
        #pragma endregion

        #pragma region Private methods
        size_type to_aligned(const size_type n) const
        {
            int alignment = 8;
            return n + (alignment - n) % n;
        }
        bool has_capacity(const size_type n) const
        {
            return n <= ((data + s) - pointer);
        }
        #pragma endregion
    };
}