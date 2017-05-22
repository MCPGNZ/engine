#pragma once

namespace pk
{
    #pragma region Forward declarations
    struct radian;
    #pragma endregion

    struct degree
    {
        double value;
        #pragma region Operators
        operator radian() const;
        #pragma endregion
    };

    template <typename T> struct vec3;
    using vec3deg = vec3<pk::degree>;
}

pk::degree operator""_deg(long double);