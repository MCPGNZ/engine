#include "random.h"

namespace pk
{
    std::random_device random::device;
    std::mt19937 random::generator{device()};
}