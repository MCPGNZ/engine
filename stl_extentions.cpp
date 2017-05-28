#include "stl_extentions.h"

namespace std
{
    bool isValid(const int value)
    {
        return value != std::numeric_limits<int>::min();
    }
}