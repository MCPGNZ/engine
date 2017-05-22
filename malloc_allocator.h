#pragma once
#include "config.h"
#include "memory_block.h"

namespace pk
{
    class malloc_allocator
    {
    public:
        #pragma region Public methods
        memory_block allocate(const size_type n) const;
        void deallocate(const memory_block& m) const;

        bool owns(const memory_block&) const;
        #pragma endregion
    };
}