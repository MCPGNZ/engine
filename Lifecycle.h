#pragma once
#include <functional>

namespace pk
{
    class Lifecycle
    {
    public:
        std::function<void(float)> _update;
    };
}
