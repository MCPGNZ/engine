#pragma once
#include "vec2.h"
#include "vec3.h"

namespace pk
{
    class SpriteSystem;

    class Vehicle
    {
    public:
        Vehicle(SpriteSystem& spriteSystem);

        void update(float deltaTime);

        vec3f position();

    public:
        SpriteSystem& _spriteSystem;
        int _sprite;

        vec2f _velocity = vec2f{0.0f};
        float _angle = 0.0f;
    };
}
