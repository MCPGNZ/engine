#pragma once
#include <vector>
#include "Lifecycle.h"

namespace pk
{
    class LifecycleSystem
    {
    public:
        void update(float deltaTime);

    private:
        std::vector<Lifecycle> _components;
    };
}
