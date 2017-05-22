#pragma once
#include <math.h>
#include "math.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace pk
{
    #pragma region Forward declaration
    struct radian;
    #pragma endregion

    __declspec(align(64)) class matrix4x4
    {
    public:
        #pragma warning (disable: 4201)
        union
        {
            float v[16];
            float m[4][4];
            struct
            {
                float m11, m21, m31, m41;
                float m12, m22, m32, m42;
                float m13, m23, m33, m43;
                float m14, m24, m34, m44;
            };
        };
        #pragma warning (default: 4201)
    public:

        #pragma region Ctors
        matrix4x4(void);
        matrix4x4(const float v);
        matrix4x4(const float n11, const float n12, const float n13, const float n14,
            const float n21, const float n22, const float n23, const float n24,
            const float n31, const float n32, const float n33, const float n34,
            const float n41, const float n42, const float n43, const float n44);

        matrix4x4(const float* u);
        #pragma endregion

        #pragma region Methods
        vec3f translation() const;
        vec3f scale() const;
        #pragma endregion

        #pragma region Operators
        bool operator==(const matrix4x4& rhs) const;
        bool operator!=(const matrix4x4& rhs) const;

        float& operator[] (const int i);
        float operator[] (const int i) const;
        float& operator() (const int row, const int column);
        float operator() (const int row, const int column) const;

        matrix4x4 operator-() const;
        matrix4x4 operator+() const;
        matrix4x4 operator+ (const matrix4x4& rhs) const;
        matrix4x4 operator- (const matrix4x4& rhs) const;

        matrix4x4 operator+ (const float& scalar) const;
        matrix4x4 operator- (const float& scalar) const;
        matrix4x4 operator* (const float& scalar) const;
        matrix4x4 operator/ (const float& scalar) const;

        matrix4x4& operator+= (const matrix4x4& rhs);
        matrix4x4& operator-= (const matrix4x4& rhs);

        matrix4x4& operator+= (const float& scalar);
        matrix4x4& operator-= (const float& scalar);
        matrix4x4& operator*= (const float& scalar);
        matrix4x4& operator/= (const float& scalar);
        #pragma endregion

        #pragma region Statics
        static  matrix4x4 identity;
        #pragma endregion
    };
    matrix4x4 operator* (const float& scalar, const matrix4x4& variable);
    matrix4x4 operator*(const matrix4x4& a, const matrix4x4& b);

    vec3f operator*(const vec3f& v, const matrix4x4& m);

    namespace math
    {
        matrix4x4 trs(const vec3f& position, const vec3<radian>& angles, const vec3f& scale);

        matrix4x4 rotation_x(const radian& angle);
        matrix4x4 rotation_y(const radian& angle);
        matrix4x4 rotation_z(const radian& angle);

        matrix4x4 rotation(const vec3<radian>& xyz);
        matrix4x4 transpose(const matrix4x4& m);

        float determinant(const matrix4x4& m);
        matrix4x4 inverse(const matrix4x4& m);

        matrix4x4 scale(const vec3f& scale);
        matrix4x4 translation(const vec3f& translation);

        vec3f scale(const matrix4x4& m);
        vec3f translation(const matrix4x4& m);

        matrix4x4 look(const vec3f& origin, const vec3f& direction, const vec3f& up);
        matrix4x4 orthogonal(const vec2f& size, const vec2f& z);
        matrix4x4 perspective(const radian& fov, const float aspect, const vec2f& z);
    }
}
