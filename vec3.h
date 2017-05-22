#pragma once
#include <cmath>
#include <stdlib.h>

#include "random.h"

namespace pk
{
    template <typename T>
    struct  vec3
    {
    public:
        #pragma warning (disable: 4201)
        union
        {
            T v[3];
            struct
            {
                T x;
                T y;
                T z;
            };
            struct
            {
                T r;
                T g;
                T b;
            };
            struct
            {
                T width;
                T height;
                T depth;
            };
        };
        #pragma warning (default: 4201)
    public:
        #pragma region Ctors
        vec3(void) : x{0}, y{0}, z{0} {}
        vec3(T v) : x{v}, y{v}, z{v} {}
        vec3(T n_x, T n_y, T n_z) : x{n_x}, y{n_y}, z{n_z} {}
        vec3(const vec3& n) : x{n.x}, y{n.y}, z{n.z} {}
        #pragma endregion

        #pragma region Operators
        bool operator== (const vec3& rhs) const;
        bool operator!= (const vec3& rhs) const;

        T operator[] (const int i) const;
        T& operator[](const int i);

        vec3 operator-() const;
        vec3 operator+() const;

        vec3 operator+ (const vec3& rhs) const;
        vec3 operator- (const vec3& rhs) const;
        vec3 operator* (const vec3& rhs) const;
        vec3 operator/ (const vec3& rhs) const;

        vec3& operator+= (const vec3& rhs);
        vec3& operator-= (const vec3& rhs);
        vec3& operator*= (const vec3& rhs);
        vec3& operator/= (const vec3& rhs);

        vec3 operator+ (const T rhs) const;
        vec3 operator- (const T rhs) const;
        vec3 operator* (const T rhs) const;
        vec3 operator/ (const T rhs) const;

        vec3& operator+= (const T rhs);
        vec3& operator-= (const T rhs);
        vec3& operator*= (const T rhs);
        vec3& operator/= (const T rhs);
        #pragma endregion

        #pragma region Casts
        template <typename U>
        explicit operator vec3<U>() const
        {
            return vec3<U>{static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
        }

        template <typename U>
        static vec3<T> cast(const U other)
        {
            return vec3<T>{other.x, other.y, other.z};
        }
        #pragma endregion

        #pragma region Statics
        static vec3 zero;
        static vec3 one;

        static vec3 right;
        static vec3 up;
        static vec3 forward;
        #pragma endregion
    };
    template <typename T> vec3<T> operator+(T scalar, const vec3<T>& rhs);
    template <typename T> vec3<T> operator*(T scalar, const vec3<T>& rhs);
    template <typename T> vec3<T> operator/(T scalar, const vec3<T>& rhs);

    #pragma region Aliases
    using vec3d = vec3<double>;
    using vec3f = vec3<float>;
    using vec3i = vec3<std::int32_t>;
    using vec3u = vec3<std::uint32_t>;
    using vec3u8 = vec3<std::uint8_t>;
    #pragma endregion

    #pragma region Statics
    template <typename T> vec3<T> vec3<T>::zero = vec3<T>{0.0f};
    template <typename T> vec3<T> vec3<T>::one = vec3<T>{1.0f};
    template <typename T> vec3<T> vec3<T>::right = vec3<T>{1.0f, 0.0f, 0.0f};
    template <typename T> vec3<T> vec3<T>::up = vec3<T>{0.0f, 1.0f, 0.0f};
    template <typename T> vec3<T> vec3<T>::forward = vec3<T>{0.0f, 0.0f, 1.0f};
    #pragma endregion
}

#pragma region Implementation
#include <cmath>
#include <stdlib.h>

namespace pk
{
    template <typename T> bool vec3<T>::operator==(const vec3<T>& rhs) const
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }
    template <typename T> bool vec3<T>::operator!=(const vec3<T>& rhs) const
    {
        return (x != rhs.x || y != rhs.y || z != rhs.z);
    }

    template <typename T> T vec3<T>::operator[](const int i) const
    {
        return v[i];
    }
    template <typename T> T& vec3<T>::operator[](const int i)
    {
        return v[i];
    }

    template <typename T> vec3<T> vec3<T>::operator-() const
    {
        return vec3{-x, -y, -z};
    }
    template <typename T> vec3<T> vec3<T>::operator+() const
    {
        return *this;
    }

    template <typename T> vec3<T> vec3<T>::operator+ (const vec3<T>& rhs) const
    {
        return vec3{x + rhs.x, y + rhs.y, z + rhs.z};
    }
    template <typename T> vec3<T> vec3<T>::operator- (const vec3<T>& rhs) const
    {
        return vec3{x - rhs.x, y - rhs.y, z - rhs.z};
    }
    template <typename T> vec3<T> vec3<T>::operator* (const vec3<T>& rhs) const
    {
        return vec3{x * rhs.x, y * rhs.y, z * rhs.z};
    }
    template <typename T> vec3<T> vec3<T>::operator/ (const vec3<T>& rhs) const
    {
        return vec3{x / rhs.x, y / rhs.y, z / rhs.z};
    }

    template <typename T> vec3<T>& vec3<T>::operator+= (const vec3<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    template <typename T> vec3<T>& vec3<T>::operator-= (const vec3<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    template <typename T> vec3<T>& vec3<T>::operator*= (const vec3<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
    template <typename T> vec3<T>& vec3<T>::operator/= (const vec3<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    template <typename T> vec3<T> vec3<T>::operator+ (T rhs) const
    {
        return vec3{x + rhs, y + rhs, z + rhs};
    }
    template <typename T> vec3<T> vec3<T>::operator- (T rhs) const
    {
        return vec3{x - rhs, y - rhs, z - rhs};
    }
    template <typename T> vec3<T> vec3<T>::operator* (T rhs) const
    {
        return vec3{x * rhs, y * rhs, z * rhs};
    }
    template <typename T> vec3<T> vec3<T>::operator/ (T rhs) const
    {
        const T inv = 1 / rhs;
        return vec3{x * inv, y * inv, z * inv};
    }

    template <typename T> vec3<T>& vec3<T>::operator+= (T rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }
    template <typename T> vec3<T>& vec3<T>::operator-= (T rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }
    template <typename T> vec3<T>& vec3<T>::operator*= (T rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    template <typename T> vec3<T>& vec3<T>::operator/= (T rhs)
    {
        const T inv = 1 / rhs;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    template <typename T> vec3<T> operator+(T scalar, const vec3<T>& rhs)
    {
        return rhs + scalar;
    }
    template <typename T> vec3<T> operator*(T scalar, const vec3<T>& rhs)
    {
        return rhs * scalar;
    }
    template <typename T> vec3<T> operator/(T scalar, const vec3<T>& rhs)
    {
        return vec3<T>{
            scalar / rhs.x,
                scalar / rhs.y,
                scalar / rhs.z
        };
    }

    namespace math
    {
        template <typename T> T length(const vec3<T>& v)
        {
            return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        }
        template <typename T> T length_inv(const vec3<T>& v)
        {
            return 1.0f / length(v);
        }
        template <typename T> T length_sq(const vec3<T>& v)
        {
            return v.x*v.x + v.y + v.z*v.z;
        }

        template <typename T> vec3<T> inverse(const vec3<T>& v)
        {
            return vec3{
                1.0f / v.x,
                1.0f / v.y,
                1.0f / v.z
            };
        }

        template <typename T> vec3<T> normalize(const  vec3<T>& v)
        {
            return v / length(v);
        }
        template <typename T> vec3<T> lerp(const  vec3<T>& a, const  vec3<T>& b, T s)
        {
            return a + (b - a) * s;
        }

        template <typename T> vec3<T> mod(const vec3<T>& a, const vec3<T>& b)
        {
            return vec3<T>{
                fmod(a.x, b.x),
                    fmod(a.y, b.y),
                    fmod(a.z, b.z)
            };
        }

        template <typename T> T dot(const vec3<T>& v0, const vec3<T>& v1)
        {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        template <typename T> vec3<T> cross(const vec3<T>& v0, const vec3<T>& v1)
        {
            return vec3<T>{
                v0.y * v1.z - v0.z * v1.y,
                    v0.z * v1.x - v0.x * v1.z,
                    v0.x * v1.y - v0.y * v1.x
            };
        }

        template <typename T, template <typename> typename Container>
        vec3<T> minimize(const Container<vec3<T>>& v)
        {
            auto result = v[0];

            const auto size = v.size();
            for(decltype(v.size()) i = 1; i < size; ++i)
            {
                if(v[i].x < result.x) result.x = v[i].x;
                if(v[i].y < result.y) result.y = v[i].y;
                if(v[i].z < result.z) result.z = v[i].z;
            }

            return result;
        }

        template <typename T>
        vec3<T> minimize(const vec3<T>& v)
        {
            return v;
        }

        template <typename T, typename... Args>
        vec3<T> minimize(const vec3<T>& v, Args&&... args)
        {
            auto minimum = minimize(std::forward<Args>(args)...);
            return vec3 < T > {
                minimum.x < v.x ? minimum.x : v.x,
                    minimum.y < v.y ? minimum.y : v.y,
                    minimum.z < v.z ? minimum.z : v.z
            };
        }

        template <typename T, template <typename> typename Container>
        vec3<T> maximize(const Container<vec3<T>>& v)
        {
            auto result = v[0];

            const auto size = v.size();
            for(decltype(v.size()) i = 1; i < size; ++i)
            {
                if(v[i].x > result.x) result.x = v[i].x;
                if(v[i].y > result.y) result.y = v[i].y;
                if(v[i].z > result.z) result.z = v[i].z;
            }

            return result;
        }
        template <typename T>
        vec3<T> maximize(const vec3<T>& v)
        {
            return v;
        }

        template <typename T, typename... Args>
        vec3<T> maximize(const vec3<T>& v, Args&&... args)
        {
            vec3<T> maximum = maximize(std::forward<Args>(args)...);
            return vec3 < T > {
                maximum.x > v.x ? maximum.x : v.x,
                    maximum.y > v.y ? maximum.y : v.y,
                    maximum.z > v.z ? maximum.z : v.z
            };
        }

        template <typename T> vec3<T> clamp(const vec3<T>& v, const vec3<T>& minimum, const vec3<T>& maximum)
        {
            return vec3<T> {
                std::min(std::max(v.x, minimum.x), maximum.x),
                    std::min(std::max(v.y, minimum.y), maximum.y),
                    std::min(std::max(v.z, minimum.z), maximum.z)
            };
        }

        template <typename T> vec3<T> random(const vec3<T>& minValue, const vec3<T>& maxValue)
        {
            vec3<T> result;
            result.x = random::range(minValue.x, maxValue.x);
            result.y = random::range(minValue.y, maxValue.y);
            result.z = random::range(minValue.z, maxValue.z);

            return result;
        }
        template <typename T> vec3<T> abs(const vec3<T>& v)
        {
            return vec3<T>{std::abs(v.x), std::abs(v.y), std::abs(v.z)};
        }

        template <typename T>
        T sum(const vec3<T>& v)
        {
            return v.x + v.y + v.z;
        }

        template <typename T, typename... Args>
        T sum(const vec3<T>& v, Args&&... args)
        {
            return v.x + v.y + v.z + Sum(std::forward<Args>(args)...);
        }

        template <typename T, template <typename> typename Container>
        T sum(const Container<vec3<T>>& v)
        {
            T result = 0;

            for(const auto& value : v)
            {
                result += value.x + value.y + value.z;
            }

            return result;
        }
    }
}
#pragma endregion