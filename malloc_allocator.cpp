#include "malloc_allocator.h"

namespace pk
{
    #pragma region Public methods
    memory_block malloc_allocator::allocate(const size_type n) const
    {
        return memory_block{malloc(n), n};
    }

    void malloc_allocator::deallocate(const memory_block& m) const
    {
        free(m.pointer);
    }

    bool malloc_allocator::owns(const memory_block&) const
    {
        return true;
    }
    #pragma endregion Public methods
}