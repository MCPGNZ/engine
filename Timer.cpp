#include "Timer.h"

namespace pk
{
    Timer::Timer()
    {
        _timePoint = std::chrono::high_resolution_clock::now();
    }
}