#pragma once
#include <stdlib.h>
#include "vec4.h"

namespace pk
{
    struct Color
    {
        #pragma region Public types
        enum struct Mode
        {
            None,
            Clamp,
            Wrap
        };
        enum Channel
        {
            R = 0x0001,
            G = 0x0010,
            B = 0x0100,
            A = 0x1000,
            RG = R | G,
            RB = R | B,
            RA = R | A,
            GB = G | B,
            GA = G | A,
            BA = B | A,
            RGA = R | G | A,
            RGB = R | G | B,
            GBA = G | B | A,
            RGBA = R | G | B | A
        };
        #pragma endregion

        #pragma region Public variables
        #pragma warning (disable: 4201)
        union
        {
            struct
            {
                std::uint8_t r;
                std::uint8_t g;
                std::uint8_t b;
                std::uint8_t a;
            };
            std::uint32_t rgba;
        };
        #pragma warning (default: 4201)
        #pragma endregion

        #pragma region Ctors
        Color(void);

        Color(const std::uint32_t& rgba);

        template <typename T>
        Color(const T& other, IS_ARITHMETIC(T)) : Color{other, other, other, other} {};

        template <typename T>
        Color(const T& other, IS_FLOATING(decltype(T::x))) : Color{other.x, other.y, other.z, other.w} {};

        Color(const int n_r, const int n_g, const int n_b, const int n_a = 255);
        Color(const float n_r, const float n_g, const float n_b, const float n_a = 1.0f);
        #pragma endregion

        #pragma region Operators
        Color operator+(const Color& other) const;
        Color operator-(const Color& other) const;

        Color operator*(const Color& other) const;
        Color operator/(const Color& other) const;
        Color operator*(const float scalar) const;
        Color operator/(const float scalar) const;

        void operator+=(const Color& other);
        void operator-=(const Color& other);
        void operator*=(const Color& other);
        void operator/=(const Color& other);

        operator vec4f() const;
        operator vec4d() const;
        operator std::uint32_t() const;
        #pragma endregion

        #pragma region Public methods
        static Color add(const Color& lhs, const Color& rhs, const Mode mode = Mode::Clamp);
        static Color subtract(const Color& lhs, const Color& rhs, const Mode mode = Mode::Clamp);
        static Color multiply(const Color& lhs, const Color& rhs, const Mode mode = Mode::Clamp);
        static Color divide(const Color& lhs, const Color& rhs, const Mode mode = Mode::Clamp);

        static Color lerp(const Color& start, const Color& end, const float t);

        template< template<typename T, typename A = std::allocator<T>> class Collection, typename T>
        static Color lerp(const Collection<T>& Colors, const float t);

        static Color clamp(vec4f vector);
        #pragma endregion

        #pragma region Static variables
        static Color white;
        static Color black;
        static Color pink;
        static Color green;
        static Color blue;
        static Color red;
        static Color yellow;
        static Color gray;
        static Color transparent;
        static Color default;
        #pragma endregion

        #pragma region Static methods
        static Color random();
        static Color random(const Channel channel);
        #pragma endregion
    };

    #pragma region Public templates
    template< template<typename T, typename A = std::allocator<T>> class Collection, typename T>
    Color Color::lerp(const Collection<T>& Colors, const float t)
    {
        const float st = (static_cast<float>(Colors.size()) - 1.0f) * t;
        return lerp(
            Colors[static_cast<int>(st)],
            Colors[static_cast<int>(st + 1.0f)],
            st - static_cast<float>(static_cast<int>(st)));
    }
    #pragma endregion
}