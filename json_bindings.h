#pragma once
#include <vector>

#include "json.hpp"
#include "vec2.h"
#include "vec4.h"

namespace pk
{
    #pragma region vec2
    template <typename T>
    void to_json(json& j, const vec2<T>& vector)
    {
        j = json{
            {std::vector<T>{vector.x, vector.y}}
        };
    }

    template <typename T>
    void from_json(const json& j, vec2<T>& vector)
    {
        auto data = j.get<std::vector<T>>();
        vector.x = data[0];
        vector.y = data[1];
    }
    #pragma endregion

    #pragma region vec4
    template <typename T>
    void to_json(json& j, const vec4<T>& vector)
    {
        j = json{
            {std::vector<T>{vector.x, vector.y, vector.z, vector.w}}
        };
    }

    template <typename T>
    void from_json(const json& j, vec4<T>& vector)
    {
        auto data = j.get<std::vector<T>>();
        vector.x = data[0];
        vector.y = data[1];
        vector.z = data[2];
        vector.w = data[3];
    }
}