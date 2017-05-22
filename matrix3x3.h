#pragma once
#include "vec3.h"

namespace pk
{
    #pragma region Forward declaration
    struct radian;
    #pragma endregion

    class matrix3x3
    {
    public:
        #pragma warning (disable: 4201)
        union
        {
            float v[9];
            float m[3][3];
            struct
            {
                float m11, m12, m13;
                float m21, m22, m23;
                float m31, m32, m33;
            };
        };
        #pragma warning (default: 4201)
    public:

        #pragma region Ctors
        matrix3x3(void);
        matrix3x3(const float v);

        matrix3x3(const float n11, const float n12, const float n13,
            const float n21, const float n22, const float n23,
            const float n31, const float n32, const float n33);

        matrix3x3(const float* u);
        #pragma endregion

        #pragma region Operators
        bool operator==(const matrix3x3& rhs) const;
        bool operator!=(const matrix3x3& rhs) const;

        float& operator[] (const int i);
        float operator[] (const int i) const;
        float& operator() (const int row, const int column);
        float operator() (const int row, const int column) const;

        matrix3x3 operator-() const;
        matrix3x3 operator+() const;
        matrix3x3 operator+ (const matrix3x3& rhs) const;
        matrix3x3 operator- (const matrix3x3& rhs) const;

        matrix3x3 operator+ (const float& scalar) const;
        matrix3x3 operator- (const float& scalar) const;
        matrix3x3 operator* (const float& scalar) const;
        matrix3x3 operator/ (const float& scalar) const;

        matrix3x3& operator+= (const matrix3x3& rhs);
        matrix3x3& operator-= (const matrix3x3& rhs);

        matrix3x3& operator+= (const float& scalar);
        matrix3x3& operator-= (const float& scalar);
        matrix3x3& operator*= (const float& scalar);
        matrix3x3& operator/= (const float& scalar);
        #pragma endregion

        #pragma region Statics
        static  matrix3x3 identity;
        #pragma endregion
    };
    matrix3x3 operator* (const float& scalar, const matrix3x3& variable);
    matrix3x3 operator*(const matrix3x3& a, const matrix3x3& b);

    namespace math
    {
        matrix3x3 rotation3_x(const radian& angle);
        matrix3x3 rotation3_y(const radian& angle);
        matrix3x3 rotation3_z(const radian& angle);

        matrix3x3 rotation3(const vec3<radian>& xyz);

        matrix3x3 transpose(const matrix3x3& m);

        float determinant(const matrix3x3& m);
        matrix3x3 inverse(const matrix3x3& in);
    }
}