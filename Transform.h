#pragma once
#include "vec3.h"
#include "matrix4x4.h"

namespace pk
{
    __declspec(align(64)) class Transform
    {
    public:
        matrix4x4 world();

        vec3f position = vec3f::zero;
        vec3f rotation = vec3f::zero;
        vec3f scale = vec3f::one;

    private:
    };
}
