#include <math.h>
#include "matrix4x4.h"
#include "radian.h"

namespace pk
{
    #pragma region Statics
    pk::matrix4x4 matrix4x4::identity = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    #pragma endregion

    #pragma region Ctors
    matrix4x4::matrix4x4(void) :
        m11{0}, m12{0}, m13{0}, m14{0},
        m21{0}, m22{0}, m23{0}, m24{0},
        m31{0}, m32{0}, m33{0}, m34{0},
        m41{0}, m42{0}, m43{0}, m44{0}
    {}

    matrix4x4::matrix4x4(const float v) :
        m11{v}, m12{v}, m13{v}, m14{v},
        m21{v}, m22{v}, m23{v}, m24{v},
        m31{v}, m32{v}, m33{v}, m34{v},
        m41{v}, m42{v}, m43{v}, m44{v}
    {}
    matrix4x4::matrix4x4(
        const float n11, const float n12, const float n13, const float n14,
        const float n21, const float n22, const float n23, const float n24,
        const float n31, const float n32, const float n33, const float n34,
        const float n41, const float n42, const float n43, const float n44) :
        m11{n11}, m12{n21}, m13{n31}, m14{n41},
        m21{n12}, m22{n22}, m23{n32}, m24{n42},
        m31{n13}, m32{n23}, m33{n33}, m34{n43},
        m41{n14}, m42{n24}, m43{n34}, m44{n44}
    {}
    matrix4x4::matrix4x4(const float* u) :
        m11{u[0]}, m12{u[1]}, m13{u[2]}, m14{u[3]},
        m21{u[4]}, m22{u[5]}, m23{u[6]}, m24{u[7]},
        m31{u[8]}, m32{u[9]}, m33{u[10]}, m34{u[11]},
        m41{u[12]}, m42{u[13]}, m43{u[14]}, m44{u[15]}
    {}
    #pragma endregion

    #pragma region public methods
    vec3f matrix4x4::translation() const
    {
        return vec3f{m41, m42, m43};
    }
    vec3f matrix4x4::scale() const
    {
        return vec3f{
            math::length(vec3f{m11, m12, m13}),
            math::length(vec3f{m21, m22, m23}),
            math::length(vec3f{m31, m32, m33})
        };
    }
    #pragma endregion

    #pragma region Operators
    bool matrix4x4::operator==(const matrix4x4& other) const
    {
        return
            m11 == other.m11 && m12 == other.m12 && m13 == other.m13 &&  m14 == other.m14 &&
            m21 == other.m21 && m22 == other.m22 && m23 == other.m23 &&  m24 == other.m24 &&
            m31 == other.m31 && m32 == other.m32 && m33 == other.m33 &&  m34 == other.m34 &&
            m41 == other.m41 && m42 == other.m42 && m43 == other.m43 &&  m44 == other.m44;
    }
    bool matrix4x4::operator!=(const matrix4x4& other) const
    {
        return
            m11 != other.m11 || m12 != other.m12 || m13 != other.m13 || m14 != other.m14 ||
            m21 != other.m21 || m22 != other.m22 || m23 != other.m23 || m24 != other.m24 ||
            m31 != other.m31 || m32 != other.m32 || m33 != other.m33 || m34 != other.m34 ||
            m41 != other.m41 || m42 != other.m42 || m43 != other.m43 || m44 != other.m44;
    }

    float& matrix4x4::operator[] (const int i)
    {
        return v[i];
    }
    float matrix4x4::operator[] (const int i) const
    {
        return v[i];
    }
    float& matrix4x4::operator() (const int row, const int column)
    {
        return m[row][column];
    }
    float matrix4x4::operator() (const int row, const int column) const
    {
        return m[row][column];
    }

    matrix4x4 matrix4x4::operator-() const
    {
        return matrix4x4{
            -m11, -m12, -m13, -m14,
            -m21, -m22, -m23, -m24,
            -m31, -m32, -m33, -m34,
            -m41, -m42, -m43, -m44};
    }
    matrix4x4 matrix4x4::operator+() const
    {
        return *this;
    }

    matrix4x4 matrix4x4::operator+ (const matrix4x4& other) const
    {
        return matrix4x4{
            m11 + other.m11, m12 + other.m12, m13 + other.m13, m14 + other.m14,
            m21 + other.m21, m22 + other.m22, m23 + other.m23, m24 + other.m24,
            m31 + other.m31, m32 + other.m32, m33 + other.m33, m34 + other.m34,
            m41 + other.m41, m42 + other.m42, m43 + other.m43, m44 + other.m44
        };
    }
    matrix4x4 matrix4x4::operator- (const matrix4x4& other) const
    {
        return matrix4x4{
            m11 - other.m11, m12 - other.m12, m13 - other.m13, m14 - other.m14,
            m21 - other.m21, m22 - other.m22, m23 - other.m23, m24 - other.m24,
            m31 - other.m31, m32 - other.m32, m33 - other.m33, m34 - other.m34,
            m41 - other.m41, m42 - other.m42, m43 - other.m43, m44 - other.m44
        };
    }

    matrix4x4 matrix4x4::operator-(const float& scalar) const
    {
        return matrix4x4{
            m11 - scalar, m12 - scalar, m13 - scalar, m14 - scalar,
            m21 - scalar, m22 - scalar, m23 - scalar, m24 - scalar,
            m31 - scalar, m32 - scalar, m33 - scalar, m34 - scalar,
            m41 - scalar, m42 - scalar, m43 - scalar, m44 - scalar
        };
    }
    matrix4x4 matrix4x4::operator* (const float& scalar) const
    {
        return matrix4x4{
            m11 * scalar, m12 * scalar, m13 * scalar, m14 * scalar,
            m21 * scalar, m22 * scalar, m23 * scalar, m24 * scalar,
            m31 * scalar, m32 * scalar, m33 * scalar, m34 * scalar,
            m41 * scalar, m42 * scalar, m43 * scalar, m44 * scalar
        };
    }
    matrix4x4 matrix4x4::operator/ (const float& scalar) const
    {
        const float div = 1.0f / scalar;

        return matrix4x4{
            m11 * div, m12 * div, m13 * div, m14 * div,
            m21 * div, m22 * div, m23 * div, m24 * div,
            m31 * div, m32 * div, m33 * div, m34 * div,
            m41 * div, m42 * div, m43 * div, m44 * div,
        };
    }

    matrix4x4& matrix4x4::operator+= (const matrix4x4& rhs)
    {
        m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14;
        m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23; m24 += rhs.m24;
        m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33; m34 += rhs.m34;
        m41 += rhs.m41; m42 += rhs.m42; m43 += rhs.m43; m44 += rhs.m44;
        return *this;
    }
    matrix4x4& matrix4x4::operator-= (const matrix4x4& rhs)
    {
        m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13; m14 -= rhs.m14;
        m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23; m24 -= rhs.m24;
        m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33; m34 -= rhs.m34;
        m41 -= rhs.m41; m42 -= rhs.m42; m43 -= rhs.m43; m44 -= rhs.m44;
        return *this;
    }

    matrix4x4& matrix4x4::operator+= (const float& scalar)
    {
        m11 += scalar; m12 += scalar; m13 += scalar; m14 += scalar;
        m21 += scalar; m22 += scalar; m23 += scalar; m24 += scalar;
        m31 += scalar; m32 += scalar; m33 += scalar; m34 += scalar;
        m41 += scalar; m42 += scalar; m43 += scalar; m44 += scalar;
        return *this;
    }
    matrix4x4& matrix4x4::operator-= (const float& scalar)
    {
        m11 -= scalar; m12 -= scalar; m13 -= scalar; m14 -= scalar;
        m21 -= scalar; m22 -= scalar; m23 -= scalar; m24 -= scalar;
        m31 -= scalar; m32 -= scalar; m33 -= scalar; m34 -= scalar;
        m41 -= scalar; m42 -= scalar; m43 -= scalar; m44 -= scalar;
        return *this;
    }
    matrix4x4& matrix4x4::operator*= (const float& scalar)
    {
        m11 *= scalar; m12 *= scalar; m13 *= scalar; m14 *= scalar;
        m21 *= scalar; m22 *= scalar; m23 *= scalar; m24 *= scalar;
        m31 *= scalar; m32 *= scalar; m33 *= scalar; m34 *= scalar;
        m41 *= scalar; m42 *= scalar; m43 *= scalar; m44 *= scalar;
        return *this;
    }
    matrix4x4& matrix4x4::operator/= (const float& scalar)
    {
        const float div = 1.0f / scalar;

        m11 *= div; m12 *= div; m13 *= div; m14 *= div;
        m21 *= div; m22 *= div; m23 *= div; m24 *= div;
        m31 *= div; m32 *= div; m33 *= div; m34 *= div;
        m41 *= div; m42 *= div; m43 *= div; m44 *= div;
        return *this;
    }

    vec3f operator*(const vec3f& v, const matrix4x4& m)
    {
        const float norm_inv = 1.0f / (m.m41 * v.x + m.m42 * v.y + m.m43 * v.z + m.m44);

        return vec3f{
            (m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14) * norm_inv,
            (m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24) * norm_inv,
            (m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34) * norm_inv
        };
    }

    matrix4x4 operator* (const float& scalar, const matrix4x4& variable)
    {
        return variable * scalar;
    }
    matrix4x4 operator*(const matrix4x4& a, const matrix4x4& b)
    {
        matrix4x4 result{};
        for(int i = 0; i < 4; i++)
        {
            result.m[i][0] =
                a.m[i][0] * b.m[0][0] +
                a.m[i][1] * b.m[1][0] +
                a.m[i][2] * b.m[2][0] +
                a.m[i][3] * b.m[3][0];

            result.m[i][1] =
                a.m[i][0] * b.m[0][1] +
                a.m[i][1] * b.m[1][1] +
                a.m[i][2] * b.m[2][1] +
                a.m[i][3] * b.m[3][1];

            result.m[i][2] =
                a.m[i][0] * b.m[0][2] +
                a.m[i][1] * b.m[1][2] +
                a.m[i][2] * b.m[2][2] +
                a.m[i][3] * b.m[3][2];

            result.m[i][3] =
                a.m[i][0] * b.m[0][3] +
                a.m[i][1] * b.m[1][3] +
                a.m[i][2] * b.m[2][3] +
                a.m[i][3] * b.m[3][3];
        }

        return result;
    }
    #pragma endregion

    namespace math
    {
        matrix4x4 rotation_x(const radian& angle)
        {
            matrix4x4 result = matrix4x4::identity;
            result.m22 = result.m33 = std::cos(angle.value);
            result.m23 = std::sin(angle.value);
            result.m32 = -result.m23;

            return result;
        }
        matrix4x4 rotation_y(const radian& angle)
        {
            matrix4x4 result = matrix4x4::identity;
            result.m11 = result.m33 = std::cos(angle.value);
            result.m13 = std::sin(angle.value);
            result.m31 = -result.m13;

            return result;
        }
        matrix4x4 rotation_z(const radian& angle)
        {
            matrix4x4 result = matrix4x4::identity;
            result.m11 = result.m22 = std::cos(angle.value);
            result.m12 = std::sin(angle.value);
            result.m21 = -result.m12;

            return result;
        }

        matrix4x4 rotation(const vec3<radian>& xyz)
        {
            return rotation_y(xyz.y) * rotation_z(xyz.z)   * rotation_x(xyz.x);
        }
        matrix4x4 transpose(const matrix4x4& m)
        {
            return matrix4x4{
                m.m11, m.m21, m.m31, m.m41,
                m.m12, m.m22, m.m32, m.m42,
                m.m13, m.m23, m.m33, m.m43,
                m.m14, m.m24, m.m34, m.m44
            };
        }

        matrix4x4 scale(const vec3f& scale)
        {
            return matrix4x4{
                scale.x, 0.0f,    0.0f,    0.0f,
                0.0f,    scale.y, 0.0f,    0.0f,
                0.0f,    0.0f,    scale.z, 0.0f,
                0.0f,    0.0f,    0.0f,    1.0f
            };
        }
        matrix4x4 translation(const vec3f& v)
        {
            return matrix4x4{
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                v.x,  v.y,  v.z,  1.0f
            };
        }

        matrix4x4 srt(const vec3f & position, const vec3<radian>& angles, const vec3f& scaling)
        {
            return translation(position) * rotation(angles) * scale(scaling);
        }

        float determinant(const matrix4x4& m)
        {
            return
                m.v[12] * m.v[9] * m.v[6] * m.v[3] - m.v[8] * m.v[13] * m.v[6] * m.v[3] -
                m.v[12] * m.v[5] * m.v[10] * m.v[3] + m.v[4] * m.v[13] * m.v[10] * m.v[3] +
                m.v[8] * m.v[5] * m.v[14] * m.v[3] - m.v[4] * m.v[9] * m.v[14] * m.v[3] -
                m.v[12] * m.v[9] * m.v[2] * m.v[7] + m.v[8] * m.v[13] * m.v[2] * m.v[7] +
                m.v[12] * m.v[1] * m.v[10] * m.v[7] - m.v[0] * m.v[13] * m.v[10] * m.v[7] -
                m.v[8] * m.v[1] * m.v[14] * m.v[7] + m.v[0] * m.v[9] * m.v[14] * m.v[7] +
                m.v[12] * m.v[5] * m.v[2] * m.v[11] - m.v[4] * m.v[13] * m.v[2] * m.v[11] -
                m.v[12] * m.v[1] * m.v[6] * m.v[11] + m.v[0] * m.v[13] * m.v[6] * m.v[11] +
                m.v[4] * m.v[1] * m.v[14] * m.v[11] - m.v[0] * m.v[5] * m.v[14] * m.v[11] -
                m.v[8] * m.v[5] * m.v[2] * m.v[15] + m.v[4] * m.v[9] * m.v[2] * m.v[15] +
                m.v[8] * m.v[1] * m.v[6] * m.v[15] - m.v[0] * m.v[9] * m.v[6] * m.v[15] -
                m.v[4] * m.v[1] * m.v[10] * m.v[15] + m.v[0] * m.v[5] * m.v[10] * m.v[15];
        }
        matrix4x4 inverse(const matrix4x4& m)
        {
            matrix4x4 result;
            float det;
            int i;

            result.v[0] =
                m.v[5] * m.v[10] * m.v[15] -
                m.v[5] * m.v[11] * m.v[14] -
                m.v[9] * m.v[6] * m.v[15] +
                m.v[9] * m.v[7] * m.v[14] +
                m.v[13] * m.v[6] * m.v[11] -
                m.v[13] * m.v[7] * m.v[10];

            result.v[4] =
                -m.v[4] * m.v[10] * m.v[15] +
                m.v[4] * m.v[11] * m.v[14] +
                m.v[8] * m.v[6] * m.v[15] -
                m.v[8] * m.v[7] * m.v[14] -
                m.v[12] * m.v[6] * m.v[11] +
                m.v[12] * m.v[7] * m.v[10];

            result.v[8] =
                m.v[4] * m.v[9] * m.v[15] -
                m.v[4] * m.v[11] * m.v[13] -
                m.v[8] * m.v[5] * m.v[15] +
                m.v[8] * m.v[7] * m.v[13] +
                m.v[12] * m.v[5] * m.v[11] -
                m.v[12] * m.v[7] * m.v[9];

            result.v[12] =
                -m.v[4] * m.v[9] * m.v[14] +
                m.v[4] * m.v[10] * m.v[13] +
                m.v[8] * m.v[5] * m.v[14] -
                m.v[8] * m.v[6] * m.v[13] -
                m.v[12] * m.v[5] * m.v[10] +
                m.v[12] * m.v[6] * m.v[9];

            result.v[1] =
                -m.v[1] * m.v[10] * m.v[15] +
                m.v[1] * m.v[11] * m.v[14] +
                m.v[9] * m.v[2] * m.v[15] -
                m.v[9] * m.v[3] * m.v[14] -
                m.v[13] * m.v[2] * m.v[11] +
                m.v[13] * m.v[3] * m.v[10];

            result.v[5] =
                m.v[0] * m.v[10] * m.v[15] -
                m.v[0] * m.v[11] * m.v[14] -
                m.v[8] * m.v[2] * m.v[15] +
                m.v[8] * m.v[3] * m.v[14] +
                m.v[12] * m.v[2] * m.v[11] -
                m.v[12] * m.v[3] * m.v[10];

            result.v[9] =
                -m.v[0] * m.v[9] * m.v[15] +
                m.v[0] * m.v[11] * m.v[13] +
                m.v[8] * m.v[1] * m.v[15] -
                m.v[8] * m.v[3] * m.v[13] -
                m.v[12] * m.v[1] * m.v[11] +
                m.v[12] * m.v[3] * m.v[9];

            result.v[13] =
                m.v[0] * m.v[9] * m.v[14] -
                m.v[0] * m.v[10] * m.v[13] -
                m.v[8] * m.v[1] * m.v[14] +
                m.v[8] * m.v[2] * m.v[13] +
                m.v[12] * m.v[1] * m.v[10] -
                m.v[12] * m.v[2] * m.v[9];

            result.v[2] =
                m.v[1] * m.v[6] * m.v[15] -
                m.v[1] * m.v[7] * m.v[14] -
                m.v[5] * m.v[2] * m.v[15] +
                m.v[5] * m.v[3] * m.v[14] +
                m.v[13] * m.v[2] * m.v[7] -
                m.v[13] * m.v[3] * m.v[6];

            result.v[6] =
                -m.v[0] * m.v[6] * m.v[15] +
                m.v[0] * m.v[7] * m.v[14] +
                m.v[4] * m.v[2] * m.v[15] -
                m.v[4] * m.v[3] * m.v[14] -
                m.v[12] * m.v[2] * m.v[7] +
                m.v[12] * m.v[3] * m.v[6];

            result.v[10] =
                m.v[0] * m.v[5] * m.v[15] -
                m.v[0] * m.v[7] * m.v[13] -
                m.v[4] * m.v[1] * m.v[15] +
                m.v[4] * m.v[3] * m.v[13] +
                m.v[12] * m.v[1] * m.v[7] -
                m.v[12] * m.v[3] * m.v[5];

            result.v[14] =
                -m.v[0] * m.v[5] * m.v[14] +
                m.v[0] * m.v[6] * m.v[13] +
                m.v[4] * m.v[1] * m.v[14] -
                m.v[4] * m.v[2] * m.v[13] -
                m.v[12] * m.v[1] * m.v[6] +
                m.v[12] * m.v[2] * m.v[5];

            result.v[3] =
                -m.v[1] * m.v[6] * m.v[11] +
                m.v[1] * m.v[7] * m.v[10] +
                m.v[5] * m.v[2] * m.v[11] -
                m.v[5] * m.v[3] * m.v[10] -
                m.v[9] * m.v[2] * m.v[7] +
                m.v[9] * m.v[3] * m.v[6];

            result.v[7] =
                m.v[0] * m.v[6] * m.v[11] -
                m.v[0] * m.v[7] * m.v[10] -
                m.v[4] * m.v[2] * m.v[11] +
                m.v[4] * m.v[3] * m.v[10] +
                m.v[8] * m.v[2] * m.v[7] -
                m.v[8] * m.v[3] * m.v[6];

            result.v[11] =
                -m.v[0] * m.v[5] * m.v[11] +
                m.v[0] * m.v[7] * m.v[9] +
                m.v[4] * m.v[1] * m.v[11] -
                m.v[4] * m.v[3] * m.v[9] -
                m.v[8] * m.v[1] * m.v[7] +
                m.v[8] * m.v[3] * m.v[5];

            result.v[15] =
                m.v[0] * m.v[5] * m.v[10] -
                m.v[0] * m.v[6] * m.v[9] -
                m.v[4] * m.v[1] * m.v[10] +
                m.v[4] * m.v[2] * m.v[9] +
                m.v[8] * m.v[1] * m.v[6] -
                m.v[8] * m.v[2] * m.v[5];

            det = m.v[0] * result.v[0] + m.v[1] * result.v[4] + m.v[2] * result.v[8] + m.v[3] * result.v[12];
            det = 1.0 / det;
            for(i = 0; i < 16; i++)
                result.v[i] *= det;

            return result;
        }

        vec3f translation(const matrix4x4& in)
        {
            return vec3f{in.m41, in.m42, in.m43};
        }
        vec3f scale(const matrix4x4& in)
        {
            return vec3f{
                math::length(vec3f{in.m11, in.m12, in.m13}),
                math::length(vec3f{in.m21, in.m22, in.m23}),
                math::length(vec3f{in.m31, in.m32, in.m33})
            };
        }

        matrix4x4 look(const vec3f& origin, const vec3f& direction = vec3f::forward, const vec3f& up = vec3f::up)
        {
            const auto z_axis = math::normalize(direction - origin);
            auto x_axis = math::cross(up, z_axis);
            auto y_axis = math::cross(z_axis, x_axis);

            x_axis = math::normalize(x_axis);
            y_axis = math::normalize(y_axis);

            const auto dv = vec3f{
                -math::dot(x_axis, origin),
                -math::dot(y_axis, origin),
                -math::dot(z_axis, origin)};

            return matrix4x4{
                x_axis.x, y_axis.x, z_axis.x, 0.0f,
                x_axis.y, y_axis.y, z_axis.y, 0.0f,
                x_axis.z, y_axis.z, z_axis.z, 0.0f,
                dv.x, dv.y,  dv.z, 1.0f
            };
        }

        matrix4x4 orthogonal(const vec2f& size, const vec2f& z)
        {
            const float near = z.x;
            const float far = z.y;

            return matrix4x4{
                2.0f / size.width, 0.0f,               0.0f, 0.0f,
                0.0f,              2.0f / size.height, 0.0f, 0.0f,
                0.0f,              0.0f,               1.0f / (far - near) , 0.0f,
                0.0f,              0.0f,               near / (near - far), 1.0f
            };
        }
        matrix4x4 perspective(const radian& fov, const float aspect, const vec2f& z)
        {
            const auto tan_half_fov = static_cast<float>(tan(fov.value * 0.5f));
            const auto inv_depth = 1.0f / (z.y - z.x);
            return matrix4x4{
                1.0f / (aspect * tan_half_fov), 0.0f,                   0.0f,                          0.0f,
                0.0f,                           1.0f / tan_half_fov,    0.0f,                          0.0f,
                0.0f,                           0.0f,                   z.y * inv_depth,             1.0f,
                0.0f,                           0.0f,                   -(z.x * z.y) * inv_depth, 0.0f
            };
        }
    }
}