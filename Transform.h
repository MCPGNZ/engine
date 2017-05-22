#pragma once
#include "vec3.h"
#include "matrix4x4.h"

namespace pk
{
    __declspec(align(64)) class Transform
    {
    public:
        matrix4x4 world();

        vec3d position;
        vec3d rotation;
        vec3d scale;
    private:
    };
}
