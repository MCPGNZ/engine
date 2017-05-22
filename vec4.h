#pragma once

#include <math.h>
#include <stdlib.h>

#include "random.h"
#include "traits.h"

namespace pk
{
    template <typename T>
    struct vec4
    {
    public:
        #pragma warning (disable: 4201)
        union
        {
            T v[4];
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };
            struct
            {
                T r;
                T g;
                T b;
                T a;
            };
        };
        #pragma warning (default: 4201)
    public:

        #pragma region Ctors
        vec4(void) : x{0}, y{0}, z{0}, w{0} {}
        vec4(T v) : x{v}, y{v}, z{v}, w{v} {}
        vec4(T n_x, T n_y, T n_z, T n_w) : x{n_x}, y{n_y}, z{n_z}, w{n_w} {}
        vec4(const vec4& n) : x{n.x}, y{n.y}, z{n.z}, w{n.w} {}
        #pragma endregion

        #pragma region Operators
        bool operator== (const vec4& rhs) const;
        bool operator!= (const vec4& rhs) const;

        T operator[] (const int i) const;
        T& operator[](const int i);

        vec4 operator-() const;
        vec4 operator+() const;

        vec4 operator+ (const vec4& rhs) const;
        vec4 operator- (const vec4& rhs) const;
        vec4 operator* (const vec4& rhs) const;
        vec4 operator/ (const vec4& rhs) const;

        vec4& operator+= (const vec4& rhs);
        vec4& operator-= (const vec4& rhs);
        vec4& operator*= (const vec4& rhs);
        vec4& operator/= (const vec4& rhs);

        vec4 operator+ (const T rhs) const;
        vec4 operator- (const T rhs) const;
        vec4 operator* (const T rhs) const;
        vec4 operator/ (const T rhs) const;

        vec4& operator+= (const T rhs);
        vec4& operator-= (const T rhs);
        vec4& operator*= (const T rhs);
        vec4& operator/= (const T rhs);
        #pragma endregion

        #pragma region Casts
        template <typename U>
        explicit operator vec4<U>() const
        {
            return vec4<U>{static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w)};
        }

        template <typename U>
        static vec4<T> cast(const U other)
        {
            return vec4<T>{other.x, other.y, other.z, other.w};
        }
        #pragma endregion

        #pragma region Statics
        static vec4 zero;
        static vec4 one;
        #pragma endregion
    };
    template <typename T> vec4<T> operator+(const T scalar, const vec4<T>& rhs);
    template <typename T> vec4<T> operator*(const T scalar, const vec4<T>& rhs);
    template <typename T> vec4<T> operator/(const T scalar, const vec4<T>& rhs);

    #pragma region Aliases
    using vec4d = vec4<double>;
    using vec4f = vec4<float>;
    using vec4i = vec4<std::int32_t>;
    using vec4u = vec4<std::uint32_t>;
    using vec4u8 = vec4<std::uint8_t>;
    #pragma endregion

    #pragma region Statics
    template <typename T> vec4<T> vec4<T>::zero = vec4<T>{0.0f};
    template <typename T> vec4<T> vec4<T>::one = vec4<T>{1.0f};
    #pragma endregion
}


#pragma region Implementation
#include <cmath>
#include <stdlib.h>

namespace pk
{
    template <typename T> bool vec4<T>::operator==(const vec4<T>& rhs) const
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
    }
    template <typename T> bool vec4<T>::operator!=(const vec4<T>& rhs) const
    {
        return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
    }

    template <typename T> T vec4<T>::operator[](const int i) const
    {
        return v[i];
    }
    template <typename T> T& vec4<T>::operator[](const int i)
    {
        return v[i];
    }

    template <typename T> vec4<T> vec4<T>::operator-() const
    {
        return vec4{-x, -y, -z, -w};
    }
    template <typename T> vec4<T> vec4<T>::operator+() const
    {
        return *this;
    }

    template <typename T> vec4<T> vec4<T>::operator+ (const vec4<T>& rhs) const
    {
        return vec4{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
    }
    template <typename T> vec4<T> vec4<T>::operator- (const vec4<T>& rhs) const
    {
        return vec4{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
    }
    template <typename T> vec4<T> vec4<T>::operator* (const vec4<T>& rhs) const
    {
        return vec4{x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w};
    }
    template <typename T> vec4<T> vec4<T>::operator/ (const vec4<T>& rhs) const
    {
        return vec4{x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w};
    }

    template <typename T> vec4<T>& vec4<T>::operator+= (const vec4<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    template <typename T> vec4<T>& vec4<T>::operator-= (const vec4<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    template <typename T> vec4<T>& vec4<T>::operator*= (const vec4<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }
    template <typename T> vec4<T>& vec4<T>::operator/= (const vec4<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    template <typename T> vec4<T> vec4<T>::operator+ (const T rhs) const
    {
        return vec4{x + rhs, y + rhs, z + rhs, w + rhs};
    }
    template <typename T> vec4<T> vec4<T>::operator- (const T rhs) const
    {
        return vec4{x - rhs, y - rhs, z - rhs, w - rhs};
    }
    template <typename T> vec4<T> vec4<T>::operator* (const T rhs) const
    {
        return vec4{x * rhs, y * rhs, z * rhs, w * rhs};
    }
    template <typename T> vec4<T> vec4<T>::operator/ (const T rhs) const
    {
        const T inv = 1 / rhs;
        return vec4{x * inv, y * inv, z * inv, w * inv};
    }

    template <typename T> vec4<T>& vec4<T>::operator+= (const T rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        w += rhs;
        return *this;
    }
    template <typename T> vec4<T>& vec4<T>::operator-= (const T rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        w -= rhs;
        return *this;
    }
    template <typename T> vec4<T>& vec4<T>::operator*= (const T rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }
    template <typename T> vec4<T>& vec4<T>::operator/= (const T rhs)
    {
        const T inv = 1 / rhs;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
        return *this;
    }

    template <typename T> vec4<T> operator+(const T scalar, const vec4<T>& rhs)
    {
        return rhs + scalar;
    }
    template <typename T> vec4<T> operator*(const T scalar, const vec4<T>& rhs)
    {
        return rhs * scalar;
    }
    template <typename T> vec4<T> operator/(const T scalar, const vec4<T>& rhs)
    {
        return vec4<T>{
            scalar / rhs.x,
                scalar / rhs.y,
                scalar / rhs.z,
                scalar / rhs.w
        };
    }

    namespace math
    {
        template <typename T> T length(const vec4<T>& v)
        {
            return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
        }
        template <typename T> T length_inv(const vec4<T>& v)
        {
            return 1.0f / length(v);
        }
        template <typename T> T length_sq(const vec4<T>& v)
        {
            return v.x*v.x + v.y + v.z*v.z + v.w*v.w;
        }

        template <typename T> vec4<T> inverse(const vec4<T>& v)
        {
            return vec4{
                1.0f / v.x,
                1.0f / v.y,
                1.0f / v.z,
                1.0f / v.w
            };
        }

        template <typename T> vec4<T> normalize(const vec4<T>& v)
        {
            return v / length(v);
        }
        template <typename T> vec4<T> lerp(const vec4<T>& a, const vec4<T>& b, T s)
        {
            return a + (b - a) * s;
        }

        template <typename T> vec4<T> mod(const vec4<T>& a, const vec4<T>& b)
        {
            return vec4<T>{
                a.x - b.x * floor(a.x / b.x),
                    a.y - b.y * floor(a.y / b.y),
                    a.z - b.z * floor(a.z / b.z),
                    a.w - b.w * floor(a.w / b.w)
            };
        }

        template <typename T, template <typename> typename Container>
        vec4<T> minimize(const Container<vec4<T>>& v)
        {
            auto result = v[0];

            const auto size = v.size();
            for(decltype(v.size()) i = 1; i < size; ++i)
            {
                if(v[i].x < result.x) result.x = v[i].x;
                if(v[i].y < result.y) result.y = v[i].y;
                if(v[i].z < result.z) result.z = v[i].z;
                if(v[i].w < result.w) result.w = v[i].w;
            }

            return result;
        }

        template <typename T>
        vec4<T> minimize(const vec4<T>& v)
        {
            return v;
        }

        template <typename T, typename... Args>
        vec4<T> minimize(const vec4<T>& v, Args&&... args)
        {
            auto minimum = minimize(std::forward<Args>(args)...);
            return vec4 < T > {
                minimum.x < v.x ? minimum.x : v.x,
                    minimum.y < v.y ? minimum.y : v.y,
                    minimum.z < v.z ? minimum.z : v.z,
                    minimum.w < v.w ? minimum.w : v.w
            };
        }

        template <typename T, template <typename> typename Container>
        vec4<T> maximize(const Container<vec4<T>>& v)
        {
            auto result = v[0];

            const auto size = v.size();
            for(decltype(v.size()) i = 1; i < size; ++i)
            {
                if(v[i].x > result.x) result.x = v[i].x;
                if(v[i].y > result.y) result.y = v[i].y;
                if(v[i].z > result.z) result.z = v[i].z;
                if(v[i].w > result.w) result.w = v[i].w;
            }

            return result;
        }
        template <typename T>
        vec4<T> maximize(const vec4<T>& v)
        {
            return v;
        }

        template <typename T, typename... Args>
        vec4<T> maximize(const vec4<T>& v, Args&&... args)
        {
            vec4<T> maximum = maximize(std::forward<Args>(args)...);
            return vec4 < T > {
                maximum.x > v.x ? maximum.x : v.x,
                    maximum.y > v.y ? maximum.y : v.y,
                    maximum.z > v.z ? maximum.z : v.z,
                    maximum.w > v.w ? maximum.w : v.w
            };
        }

        template <typename T> vec4<T> clamp(const vec4<T>& v, const vec4<T>& minimum, const vec4<T>& maximum)
        {
            return vec4<T> {
                std::min(std::max(v.x, minimum.x), maximum.x),
                    std::min(std::max(v.y, minimum.y), maximum.y),
                    std::min(std::max(v.z, minimum.z), maximum.z),
                    std::min(std::max(v.w, minimum.w), maximum.w),
            };
        }

        template <typename T> vec4<T> random(const vec4<T>& minValue, const vec4<T>& maxValue)
        {
            vec4<T> result;
            result.x = random::range(minValue.x, maxValue.x);
            result.y = random::range(minValue.y, maxValue.y);
            result.z = random::range(minValue.z, maxValue.z);
            result.w = random::range(minValue.w, maxValue.w);

            return result;
        }
        template <typename T> vec4<T> abs(const vec4<T>& v)
        {
            return vec4<T>{std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w)};
        }

        template <typename T>
        T sum(const vec4<T>& v)
        {
            return v.x + v.y + v.z + v.w;
        }

        template <typename T, typename... Args>
        T sum(const vec4<T>& v, Args&&... args)
        {
            return v.x + v.y + v.z + v.w + Sum(std::forward<Args>(args)...);
        }

        template <typename T, template <typename> typename Container>
        T sum(const Container<vec4<T>>& v)
        {
            T result = 0;

            for(const auto& value : v)
            {
                result += value.x + value.y + value.z + value.w;
            }

            return result;
        }
    }
}
#pragma endregion