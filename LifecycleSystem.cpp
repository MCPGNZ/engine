#include "LifecycleSystem.h"


namespace pk
{
    void LifecycleSystem::update(float deltaTime)
    {
        for(auto& component : _components)
        {
            component._update(deltaTime);
        }
    }
}
