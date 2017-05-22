#pragma once

#include "vec2.h"

namespace pk
{
    class Renderer;

    class Viewport
    {
    public:
        #pragma region Ctors
        Viewport(void) = default;
        Viewport(const vec2f& position, const vec2f& scale);
        #pragma endregion

        #pragma region Public variables
        vec2f position = vec2f{0.0f, 0.0f};
        vec2f scale = vec2f{1.0f, 1.0f};
        #pragma endregion
    };
}