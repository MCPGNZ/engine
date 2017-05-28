#pragma once
#include "vec3.h"
#include "matrix4x4.h"

#include "json.hpp"
#include "json_bindings.h"

namespace pk
{
    class Transform
    {
    public:
        matrix4x4 worldMatrix() const;
        matrix4x4 rotationMatrix() const;

        vec3f forward() const;

        vec3f position = vec3f::zero;
        vec3f rotation = vec3f::zero;
        vec3f scale = vec3f::one;
    private:
    };

    #pragma region serialization
    void to_json(json& j, const pk::Transform& transform);
    void from_json(const json& j, pk::Transform& transform);
    #pragma endregion
}