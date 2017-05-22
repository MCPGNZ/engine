#pragma once

namespace pk
{
    #pragma region Forward declarations
    struct degree;
    #pragma endregion

    struct radian
    {
        double value;

        #pragma region Operators
        operator degree() const;
        #pragma endregion
    };

    template <typename T> struct vec3;
    using vec3r = vec3<radian>;
}

pk::radian operator""_rad(long double value);