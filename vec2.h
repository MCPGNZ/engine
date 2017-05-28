#pragma once
#include <cmath>
#include <cstdint>
#include <stdlib.h>

namespace pk
{
    template <typename T>
    struct vec2
    {
    public:
        #pragma warning (disable: 4201)
        union
        {
            T v[2];
            struct
            {
                T x;
                T y;
            };
            struct
            {
                T r;
                T g;
            };
            struct
            {
                T width;
                T height;
            };
        };
        #pragma warning (default: 4201)
    public:
        #pragma region Ctors
        vec2(void) : x{0}, y{0} {};
        vec2(T v) : x{v}, y{v} {};
        vec2(T n_x, T n_y) : x{n_x}, y{n_y} {};
        vec2(const vec2& n) : x{n.x}, y{n.y} {};
        #pragma endregion

        #pragma region Operators
        bool operator== (const vec2& rhs) const;
        bool operator!= (const vec2& rhs) const;

        T operator[] (const int i) const;
        T& operator[](const int i);

        vec2 operator-() const;
        vec2 operator+() const;

        vec2 operator+ (const vec2& rhs) const;
        vec2 operator- (const vec2& rhs) const;
        vec2 operator* (const vec2& rhs) const;
        vec2 operator/ (const vec2& rhs) const;

        vec2& operator+= (const vec2& rhs);
        vec2& operator-= (const vec2& rhs);
        vec2& operator*= (const vec2& rhs);
        vec2& operator/= (const vec2& rhs);


        template <typename U> vec2 operator+ (const U rhs) const;
        template <typename U> vec2 operator- (const U rhs) const;
        template <typename U> vec2 operator* (const U rhs) const;
        template <typename U> vec2 operator/ (const U rhs) const;

        template <typename U> vec2& operator+= (const U rhs);
        template <typename U> vec2& operator-= (const U rhs);
        template <typename U> vec2& operator*= (const U rhs);
        template <typename U> vec2& operator/= (const U rhs);
        #pragma endregion

        #pragma region Casts
        template <typename U>
        explicit operator vec2<U>() const
        {
            return vec2<U>{static_cast<U>(x), static_cast<U>(y)};
        }

        template <typename U>
        static vec2<T> cast(U other)
        {
            return vec2<T>{static_cast<T>(other.x), static_cast<T>(other.y)};
        }
        #pragma endregion

        static vec2 maximum();
        static vec2 minimum();
    };
    template <typename T> vec2<T> operator+(const T scalar, const vec2<T>& rhs);
    template <typename T> vec2<T> operator*(const T scalar, const vec2<T>& rhs);
    template <typename T> vec2<T> operator/(const T scalar, const vec2<T>& rhs);

    #pragma region Aliases
    using vec2d = vec2<double>;
    using vec2f = vec2<float>;
    using vec2i = vec2<std::int32_t>;
    using vec2u = vec2<std::uint32_t>;
    using vec2u8 = vec2<std::uint8_t>;
    #pragma endregion

    template <typename T> vec2<T> vec2<T>::maximum();
    template <typename T> vec2<T> vec2<T>::minimum();
}


#pragma region Implementation
namespace pk
{
    template <typename T> vec2<T> vec2<T>::maximum()
    {
        return vec2<T>{
            std::numeric_limits<T>::max(),
                std::numeric_limits<T>::max()
        };
    }
    template <typename T> vec2<T> vec2<T>::minimum()
    {
        return vec2<T>{
            std::numeric_limits<T>::min(),
                std::numeric_limits<T>::min()
        };
    }

    template <typename T> bool vec2<T>::operator==(const vec2<T>& rhs) const
    {
        return (x == rhs.x && y == rhs.y);
    }
    template <typename T> bool vec2<T>::operator!=(const vec2<T>& rhs) const
    {
        return (x != rhs.x || y != rhs.y);
    }

    template <typename T> T vec2<T>::operator[](const int i) const
    {
        return v[i];
    }
    template <typename T> T& vec2<T>::operator[](const int i)
    {
        return v[i];
    }

    template <typename T> vec2<T> vec2<T>::operator-() const
    {
        return vec2{-x, -y};
    }
    template <typename T> vec2<T> vec2<T>::operator+() const
    {
        return *this;
    }

    template <typename T> vec2<T> vec2<T>::operator+ (const vec2<T>& rhs) const
    {
        return vec2{x + rhs.x, y + rhs.y};
    }
    template <typename T> vec2<T> vec2<T>::operator- (const vec2<T>& rhs) const
    {
        return vec2{x - rhs.x, y - rhs.y};
    }
    template <typename T> vec2<T> vec2<T>::operator* (const vec2<T>& rhs) const
    {
        return vec2{x * rhs.x, y * rhs.y};
    }

    template <typename T> vec2<T> vec2<T>::operator/ (const vec2<T>& rhs) const
    {
        return vec2{x / rhs.x, y / rhs.y};
    }

    template <typename T> vec2<T>& vec2<T>::operator+= (const vec2<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    template <typename T> vec2<T>& vec2<T>::operator-= (const vec2<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    template <typename T> vec2<T>& vec2<T>::operator*= (const vec2<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    template <typename T> vec2<T>& vec2<T>::operator/= (const vec2<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }


    template <typename T> template <typename U> vec2<T> vec2<T>::operator+ (const U rhs) const
    {
        return vec2{(T) (x + rhs), (T) (y + rhs)};
    }
    template <typename T> template <typename U> vec2<T> vec2<T>::operator- (const U rhs) const
    {
        return vec2{(T) (x - rhs), (T) (y - rhs)};
    }
    template <typename T> template <typename U> vec2<T> vec2<T>::operator* (const U rhs) const
    {
        return vec2{(T) (x * rhs), (T) (y * rhs)};
    }
    template <typename T> template <typename U> vec2<T> vec2<T>::operator/ (const U rhs) const
    {
        const U inv = 1 / rhs;
        return vec2{(T) (x * inv),(T) (y * inv)};
    }

    template <typename T> template <typename U> vec2<T>& vec2<T>::operator+= (const U rhs)
    {
        x += rhs;
        y += rhs;
        return *this;
    }
    template <typename T> template <typename U> vec2<T>& vec2<T>::operator-= (const U rhs)
    {
        x -= rhs;
        y -= rhs;
        return *this;
    }
    template <typename T> template <typename U> vec2<T>& vec2<T>::operator*= (const U rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    template <typename T> template <typename U> vec2<T>& vec2<T>::operator/= (const U rhs)
    {
        const T inv = 1 / rhs;
        x *= inv;
        y *= inv;
        return *this;
    }

    template <typename T> vec2<T> operator+(const T scalar, const vec2<T>& rhs)
    {
        return rhs + scalar;
    }
    template <typename T> vec2<T> operator*(const T scalar, const vec2<T>& rhs)
    {
        return rhs * scalar;
    }
    template <typename T> vec2<T> operator/(const T scalar, const vec2<T>& rhs)
    {
        return vec2<T>{
            scalar / rhs.x,
                scalar / rhs.y,
        };
    }

    namespace math
    {
        template <typename T> T length(const vec2<T>& v)
        {
            return sqrtf(v.x*v.x + v.y*v.y);
        }
        template <typename T> T length_inv(const vec2<T>& v)
        {
            return 1.0f / length(v);
        }
        template <typename T> T length_sq(const vec2<T>& v)
        {
            return v.x*v.x + v.y;
        }

        template <typename T> vec2<T> inverse(const vec2<T>& v)
        {
            return vec2{
                1.0f / v.x,
                1.0f / v.y
            };
        }

        template <typename T> vec2<T> normalize(const vec2<T>& v)
        {
            return v / length(v);
        }
        template <typename T> vec2<T> lerp(const  vec2<T>& a, const  vec2<T>& b, T s)
        {
            return a + (b - a) * s;
        }

        template <typename T> vec2<T> mod(const vec2<T>& a, const vec2<T>& b)
        {
            return vec2<T>{
                a.x - b.x * floor(a.x / b.x),
                    a.y - b.y * floor(a.y / b.y)
            };
        }

        template <typename T> T dot(const vec2<T>& v0, const vec2<T>& v1)
        {
            return v0.x * v1.x + v0.y * v1.y;
        }
        template <typename T> vec2<T> perpendicular(const vec2<T>& v)
        {
            return vec2<T>{-v.y, v.x};
        }

        template <typename T, template <typename> typename Container>
        vec2<T> minimize(const Container<vec2<T>>& v)
        {
            auto result = v[0];

            const auto size = v.size();
            for(decltype(v.size()) i = 1; i < size; ++i)
            {
                if(v[i].x < result.x) result.x = v[i].x;
                if(v[i].y < result.y) result.y = v[i].y;
            }

            return result;
        }

        template <typename T>
        vec2<T> minimize(const vec2<T>& v)
        {
            return v;
        }

        template <typename T, typename... Args>
        vec2<T> minimize(const vec2<T>& v, Args&&... args)
        {
            auto minimum = minimize(std::forward<Args>(args)...);
            return vec2 < T > {
                minimum.x < v.x ? minimum.x : v.x,
                    minimum.y < v.y ? minimum.y : v.y
            };
        }

        template <typename T, template <typename> typename Container>
        vec2<T> maximize(const Container<vec2<T>>& v)
        {
            auto result = v[0];

            const auto size = v.size();
            for(decltype(v.size()) i = 1; i < size; ++i)
            {
                if(v[i].x > result.x) result.x = v[i].x;
                if(v[i].y > result.y) result.y = v[i].y;
            }

            return result;
        }

        template <typename T>
        vec2<T> maximize(const vec2<T>& v)
        {
            return v;
        }

        template <typename T, typename... Args>
        vec2<T> maximize(const vec2<T>& v, Args&&... args)
        {
            vec2<T> maximum = maximize(std::forward<Args>(args)...);
            return vec2 < T > {
                maximum.x > v.x ? maximum.x : v.x,
                    maximum.y > v.y ? maximum.y : v.y
            };
        }

        template <typename T> vec2<T> clamp(const vec2<T>& v, const vec2<T>& minimum, const vec2<T>& maximum)
        {
            return vec2<T> {
                std::min(std::max(v.x, minimum.x), maximum.x),
                    std::min(std::max(v.y, minimum.y), maximum.y)
            };
        }

        template <typename T> vec2<T> random(const vec2<T>& minValue, const vec2<T>& maxValue)
        {
            vec2<T> result;
            result.x = random::range(minValue.x, maxValue.x);
            result.y = random::range(minValue.y, maxValue.y);

            return result;
        }
        template <typename T> vec2<T> abs(const vec2<T>& v)
        {
            return vec2<T>{std::abs(v.x), std::abs(v.y)};
        }

        template <typename T>
        T sum(const vec2<T>& v)
        {
            return v.x + v.y;
        }

        template <typename T, typename... Args>
        T sum(const vec2<T>& v, Args&&... args)
        {
            return v.x + v.y + Sum(std::forward<Args>(args)...);
        }

        template <typename T, template <typename> typename Container>
        T sum(const Container<vec2<T>>& v)
        {
            T result = 0;
            for(const auto& value : v)
            {
                result += value.x + value.y;
            }

            return result;
        }
    }
}
#pragma endregion 