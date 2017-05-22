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
            double v[16];
            double m[4][4];
            struct
            {
                double m11, m21, m31, m41;
                double m12, m22, m32, m42;
                double m13, m23, m33, m43;
                double m14, m24, m34, m44;
            };
        };
        #pragma warning (default: 4201)
    public:

        #pragma region Ctors
        matrix4x4(void);
        matrix4x4(const double v);
        matrix4x4(const double n11, const double n12, const double n13, const double n14,
            const double n21, const double n22, const double n23, const double n24,
            const double n31, const double n32, const double n33, const double n34,
            const double n41, const double n42, const double n43, const double n44);

        matrix4x4(const double* u);
        #pragma endregion

        #pragma region Methods
        vec3d translation() const;
        vec3d scale() const;
        #pragma endregion

        #pragma region Operators
        bool operator==(const matrix4x4& rhs) const;
        bool operator!=(const matrix4x4& rhs) const;

        double& operator[] (const int i);
        double operator[] (const int i) const;
        double& operator() (const int row, const int column);
        double operator() (const int row, const int column) const;

        matrix4x4 operator-() const;
        matrix4x4 operator+() const;
        matrix4x4 operator+ (const matrix4x4& rhs) const;
        matrix4x4 operator- (const matrix4x4& rhs) const;

        matrix4x4 operator+ (const double& scalar) const;
        matrix4x4 operator- (const double& scalar) const;
        matrix4x4 operator* (const double& scalar) const;
        matrix4x4 operator/ (const double& scalar) const;

        matrix4x4& operator+= (const matrix4x4& rhs);
        matrix4x4& operator-= (const matrix4x4& rhs);

        matrix4x4& operator+= (const double& scalar);
        matrix4x4& operator-= (const double& scalar);
        matrix4x4& operator*= (const double& scalar);
        matrix4x4& operator/= (const double& scalar);
        #pragma endregion

        #pragma region Statics
        static  matrix4x4 identity;
        #pragma endregion
    };
    matrix4x4 operator* (const double& scalar, const matrix4x4& variable);
    matrix4x4 operator*(const matrix4x4& a, const matrix4x4& b);

    vec3d operator*(const vec3d& v, const matrix4x4& m);

    namespace math
    {
        matrix4x4 trs(const vec3d& position, const vec3<radian>& angles, const vec3d& scale);

        matrix4x4 rotation_x(const radian& angle);
        matrix4x4 rotation_y(const radian& angle);
        matrix4x4 rotation_z(const radian& angle);

        matrix4x4 rotation(const vec3<radian>& xyz);
        matrix4x4 transpose(const matrix4x4& m);

        double determinant(const matrix4x4& m);
        matrix4x4 inverse(const matrix4x4& m);

        matrix4x4 scale(const vec3d& scale);
        matrix4x4 translation(const vec3d& translation);

        vec3d scale(const matrix4x4& m);
        vec3d translation(const matrix4x4& m);

        matrix4x4 look(const vec3d& origin, const vec3d& direction, const vec3d& up);
        matrix4x4 orthogonal(const vec2d& size, const vec2d& z);
        matrix4x4 perspective(const radian& fov, const double aspect, const vec2d& z);
    }
}
