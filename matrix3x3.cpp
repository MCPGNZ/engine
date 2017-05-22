#include "matrix3x3.h"
#include "radian.h"

namespace pk
{
    #pragma region Statics
    pk::matrix3x3 matrix3x3::identity = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    #pragma endregion

    #pragma region Ctors
    matrix3x3::matrix3x3(void) :
        m11{0}, m12{0}, m13{0},
        m21{0}, m22{0}, m23{0},
        m31{0}, m32{0}, m33{0}
    {}
    matrix3x3::matrix3x3(const float v) :
        m11{v}, m12{v}, m13{v},
        m21{v}, m22{v}, m23{v},
        m31{v}, m32{v}, m33{v}
    {}
    matrix3x3::matrix3x3(
        const float n11, const float n12, const float n13,
        const float n21, const float n22, const float n23,
        const float n31, const float n32, const float n33) :
        m11{n11}, m12{n12}, m13{n13},
        m21{n21}, m22{n22}, m23{n23},
        m31{n31}, m32{n32}, m33{n33}
    {}
    matrix3x3::matrix3x3(const float* u) :
        m11{u[0]}, m12{u[1]}, m13{u[2]},
        m21{u[3]}, m22{u[4]}, m23{u[5]},
        m31{u[6]}, m32{u[7]}, m33{u[8]}
    {}
    #pragma endregion

    #pragma region Operators
    bool matrix3x3::operator==(const matrix3x3& rhs) const
    {
        return m11 == rhs.m11 && m12 == rhs.m12 && m13 == rhs.m13 &&
            m21 == rhs.m21 && m22 == rhs.m22 && m23 == rhs.m23 &&
            m31 == rhs.m31 && m32 == rhs.m32 && m33 == rhs.m33;
    }
    bool matrix3x3::operator!=(const matrix3x3& rhs) const
    {
        return m11 != rhs.m11 || m12 != rhs.m12 || m13 != rhs.m13 ||
            m21 != rhs.m21 || m22 != rhs.m22 || m23 != rhs.m23 ||
            m31 != rhs.m31 || m32 != rhs.m32 || m33 != rhs.m33;
    }

    float& matrix3x3::operator[] (const int i)
    {
        return v[i];
    }
    float matrix3x3::operator[] (const int i) const
    {
        return v[i];
    }
    float& matrix3x3::operator() (const int row, const int column)
    {
        return m[row][column];
    }
    float matrix3x3::operator() (const int row, const int column) const
    {
        return m[row][column];
    }

    matrix3x3 matrix3x3::operator-() const
    {
        return matrix3x3{
            -m11, -m12, -m13,
            -m21, -m22, -m23,
            -m32, -m32, -m33
        };
    }
    matrix3x3 matrix3x3::operator+() const
    {
        return *this;
    }

    matrix3x3 matrix3x3::operator+ (const matrix3x3& rhs) const
    {
        return matrix3x3{
            m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
            m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
            m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33
        };
    }
    matrix3x3 matrix3x3::operator- (const matrix3x3& rhs) const
    {
        return matrix3x3{
            m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
            m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
            m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33
        };
    }

    matrix3x3 matrix3x3::operator+ (const float& scalar) const
    {
        return matrix3x3{
            m11 + scalar, m12 + scalar, m13 + scalar,
            m21 + scalar, m22 + scalar, m23 + scalar,
            m31 + scalar, m32 + scalar, m33 + scalar
        };
    }
    matrix3x3 matrix3x3::operator- (const float& scalar) const
    {
        return matrix3x3{
            m11 - scalar, m12 - scalar, m13 - scalar,
            m21 - scalar, m22 - scalar, m23 - scalar,
            m31 - scalar, m32 - scalar, m33 - scalar
        };
    }
    matrix3x3 matrix3x3::operator* (const float& scalar) const
    {
        return matrix3x3{
            m11 * scalar, m12 * scalar, m13 * scalar,
            m21 * scalar, m22 * scalar, m23 * scalar,
            m31 * scalar, m32 * scalar, m33 * scalar
        };
    }
    matrix3x3 matrix3x3::operator/ (const float& scalar) const
    {
        const float div = 1.0f / scalar;

        return matrix3x3{
            m11 * div, m12 * div, m13 * div,
            m21 * div, m22 * div, m23 * div,
            m31 * div, m32 * div, m33 * div
        };
    }

    matrix3x3& matrix3x3::operator+= (const matrix3x3& rhs)
    {
        m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
        m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
        m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
        return *this;
    }
    matrix3x3& matrix3x3::operator-= (const matrix3x3& rhs)
    {
        m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
        m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
        m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
        return *this;
    }

    matrix3x3& matrix3x3::operator+= (const float& scalar)
    {
        m11 += scalar; m12 += scalar; m13 += scalar;
        m21 += scalar; m22 += scalar; m23 += scalar;
        m31 += scalar; m32 += scalar; m33 += scalar;
        return *this;
    }
    matrix3x3& matrix3x3::operator-= (const float& scalar)
    {
        m11 -= scalar; m12 -= scalar; m13 -= scalar;
        m21 -= scalar; m22 -= scalar; m23 -= scalar;
        m31 -= scalar; m32 -= scalar; m33 -= scalar;
        return *this;
    }
    matrix3x3& matrix3x3::operator*= (const float& scalar)
    {
        m11 *= scalar; m12 *= scalar; m13 *= scalar;
        m21 *= scalar; m22 *= scalar; m23 *= scalar;
        m31 *= scalar; m32 *= scalar; m33 *= scalar;
        return *this;
    }
    matrix3x3& matrix3x3::operator/= (const float& scalar)
    {
        const float div = 1.0f / scalar;

        m11 *= div; m12 *= div; m13 *= div;
        m21 *= div; m22 *= div; m23 *= div;
        m31 *= div; m32 *= div; m33 *= div;
        return *this;
    }

    matrix3x3 operator* (const float& scalar, const matrix3x3& variable)
    {
        return variable * scalar;
    }
    matrix3x3 operator*(const matrix3x3& a, const matrix3x3& b)
    {
        matrix3x3 result{0.0f};

        for(int i = 0; i < 3; i++)
        {
            result.m[i][0] =
                a.m[i][0] * b.m[0][0] +
                a.m[i][1] * b.m[1][0] +
                a.m[i][2] * b.m[2][0];

            result.m[i][1] =
                a.m[i][0] * b.m[0][1] +
                a.m[i][1] * b.m[1][1] +
                a.m[i][2] * b.m[2][1];

            result.m[i][2] =
                a.m[i][0] * b.m[0][2] +
                a.m[i][1] * b.m[1][2] +
                a.m[i][2] * b.m[2][2];
        }

        return result;
    }
    #pragma endregion

    namespace math
    {
        matrix3x3 rotation3_x(const radian& angle)
        {
            matrix3x3 result = matrix3x3::identity;
            result.m22 = result.m33 = std::cos(angle.value);
            result.m23 = std::sin(angle.value);
            result.m32 = -result.m23;

            return result;
        }
        matrix3x3 rotation3_y(const radian& angle)
        {
            matrix3x3 result = matrix3x3::identity;
            result.m11 = result.m33 = std::cos(angle.value);
            result.m31 = std::sin(angle.value);
            result.m13 = -result.m31;

            return result;
        }
        matrix3x3 rotation3_z(const radian& angle)
        {
            matrix3x3 result = matrix3x3::identity;
            result.m11 = result.m22 = std::cos(angle.value);
            result.m12 = std::sin(angle.value);
            result.m21 = -result.m12;

            return result;
        }

        matrix3x3 rotation3(const vec3<radian>& xyz)
        {
            return rotation3_x(xyz.x) * rotation3_y(xyz.y) * rotation3_z(xyz.z);
        }

        matrix3x3 RotationAxisAngle(const vec3f& axis, float angle)
        {
            float s = sin(angle);
            float c = cos(angle);

            float oc = 1.0f - c;

            return matrix3x3{
                c + axis.x*axis.x*oc, axis.x*axis.y*oc - axis.z*s, axis.x*axis.z*oc + axis.y * s,
                axis.y * axis.x * oc + axis.z * s, c + axis.y*axis.y*oc, axis.y*axis.z*oc - axis.x * s,
                axis.z * axis.x * oc - axis.y * s, axis.z*axis.y*oc + axis.x * s, c + axis.z*axis.z* oc
            };
        }
        float determinant(const matrix3x3& m)
        {
            return m.m11 * m.m22 * m.m22 +
                m.m12 * m.m23 * m.m31 +
                m.m13 * m.m21 * m.m32 -
                m.m13 * m.m22 * m.m31 -
                m.m12 * m.m21 * m.m33 -
                m.m11 * m.m23 * m.m32;
        }
        matrix3x3 transpose(const matrix3x3& m)
        {
            return matrix3x3{
                m.m11, m.m21, m.m31,
                m.m12, m.m22, m.m32,
                m.m13, m.m23, m.m33
            };
        }
        matrix3x3 inverse(const matrix3x3& in)
        {
            const float det_inv = 1.0f / determinant(in);
            return matrix3x3{
                (in.m22*in.m33 - in.m23*in.m32) * det_inv,
                (in.m13*in.m32 - in.m12*in.m33) * det_inv,
                (in.m12*in.m23 - in.m13*in.m22) * det_inv,
                (in.m23*in.m31 - in.m21*in.m33) * det_inv,
                (in.m11*in.m33 - in.m13*in.m31) * det_inv,
                (in.m13*in.m21 - in.m11*in.m23) * det_inv,
                (in.m21*in.m31 - in.m22*in.m31) * det_inv,
                (in.m12*in.m31 - in.m11*in.m32) * det_inv,
                (in.m11*in.m22 - in.m12*in.m21) * det_inv
            };
        }
    }
}