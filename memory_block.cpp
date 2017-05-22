#include "memory_block.h"

namespace pk
{
    bool memory_block::is_valid() const
    {
        return (pointer != nullptr) && (size != 0);
    }

    bool memory_block::contains(memory_address memory) const
    {
        return (memory >= pointer) && (memory <= (pointer + size));
    }
}