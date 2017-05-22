#include "Color.h"
#include <algorithm>

#include "random.h"

namespace pk
{
    #pragma region Static variables
    Color Color::transparent = {0.0f, 0.0f, 0.0f, 0.0f};
    Color Color::gray = {0.5f, 0.5f, 0.5f, 1.0f};
    Color Color::black = {0.0f, 0.0f, 0.0f, 1.0f};

    Color Color::white = {1.0f, 1.0f, 1.0f, 1.0f};
    Color Color::pink = {1.0f, 0.0f, 1.0f, 1.0f};
    Color Color::green = {0.0f, 1.0f, 0.0f, 1.0f};
    Color Color::blue = {0.0f, 0.0f, 1.0f, 1.0f};
    Color Color::red = {1.0f, 0.0f, 0.0f, 1.0f};
    Color Color::yellow = {1.0f, 1.0f, 0.0f, 1.0f};

    Color Color::default = {0.70f, 0.19f, 0.68f, 1.0f};
    #pragma endregion

    #pragma region Ctors
    Color::Color(void) : r{0}, g{0}, b{0}, a{0}
    {}
    Color::Color(const std::uint32_t& n_rgba) :
        rgba{n_rgba}
    {}
    Color::Color(const int n_r, const int n_g, const int n_b, const int n_a) :
        r{static_cast<std::uint8_t>(n_r)},
        g{static_cast<std::uint8_t>(n_g)},
        b{static_cast<std::uint8_t>(n_b)},
        a{static_cast<std::uint8_t>(n_a)}
    {}
    Color::Color(const float n_r, const float n_g, const float n_b, const float n_a) :
        r{static_cast<std::uint8_t>(n_r * 255.0f)},
        g{static_cast<std::uint8_t>(n_g * 255.0f)},
        b{static_cast<std::uint8_t>(n_b * 255.0f)},
        a{static_cast<std::uint8_t>(n_a * 255.0f)}
    {}
    #pragma endregion

    #pragma region Operators
    Color Color::operator+(const Color& other) const
    {
        return Color{
            static_cast<std::uint8_t>(r + other.r),
            static_cast<std::uint8_t>(g + other.g),
            static_cast<std::uint8_t>(b + other.b),
            static_cast<std::uint8_t>(a + other.a)
        };
    }
    Color Color::operator-(const Color& other) const
    {
        return Color{
            static_cast<std::uint8_t>(r - other.r),
            static_cast<std::uint8_t>(g - other.g),
            static_cast<std::uint8_t>(b - other.b),
            static_cast<std::uint8_t>(a - other.a)
        };
    }
    Color Color::operator*(const Color& other) const
    {
        return Color{
            static_cast<std::uint8_t>(r * other.r),
            static_cast<std::uint8_t>(g * other.g),
            static_cast<std::uint8_t>(b * other.b),
            static_cast<std::uint8_t>(a * other.a)
        };
    }
    Color Color::operator/(const Color& other) const
    {
        return Color{
            static_cast<std::uint8_t>(r / other.r),
            static_cast<std::uint8_t>(g / other.g),
            static_cast<std::uint8_t>(b / other.b),
            static_cast<std::uint8_t>(a / other.a)
        };
    }

    Color Color::operator*(const float scalar) const
    {
        return Color{
            static_cast<std::uint8_t>(r * scalar),
            static_cast<std::uint8_t>(g * scalar),
            static_cast<std::uint8_t>(b * scalar),
            static_cast<std::uint8_t>(a * scalar)
        };
    }
    Color Color::operator/(const float scalar) const
    {
        const float inv = 1.0f / scalar;
        return Color{
            static_cast<std::uint8_t>(r * inv),
            static_cast<std::uint8_t>(g * inv),
            static_cast<std::uint8_t>(b * inv),
            static_cast<std::uint8_t>(a * inv)
        };
    }

    void Color::operator+=(const Color& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
    }
    void Color::operator-=(const Color& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
    }
    void Color::operator*=(const Color& other)
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;
    }
    void Color::operator/=(const Color& other)
    {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;
    }

    Color::operator vec4f() const
    {
        return {r * 0.00392156862747f, g * 0.00392156862747f, b * 0.00392156862747f, a * 0.00392156862747f};
    }
    Color::operator vec4d() const
    {
        return {r * 0.00392156862747f, g * 0.00392156862747f, b * 0.00392156862747f, a * 0.00392156862747f};
    }
    Color::operator std::uint32_t() const
    {
        return rgba;
    }
    #pragma endregion

    #pragma region Static methods
    Color Color::add(const Color& lhs, const Color& rhs, const Mode mode /*= Mode::CLAMP*/)
    {
        switch(mode)
        {
            case Mode::Clamp:
            {
                return Color{
                    static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(lhs.r) + static_cast<std::int32_t>(rhs.r), 255)),
                    static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(lhs.g) + static_cast<std::int32_t>(rhs.g), 255)),
                    static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(lhs.b) + static_cast<std::int32_t>(rhs.b), 255)),
                    static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(lhs.a) + static_cast<std::int32_t>(rhs.a), 255))
                };
            }
            case Mode::Wrap:
            {
                const auto dr = static_cast<std::uint32_t>(lhs.r) + static_cast<std::uint32_t>(rhs.r);
                const auto dg = static_cast<std::uint32_t>(lhs.g) + static_cast<std::uint32_t>(rhs.g);
                const auto db = static_cast<std::uint32_t>(lhs.b) + static_cast<std::uint32_t>(rhs.b);
                const auto da = static_cast<std::uint32_t>(lhs.a) + static_cast<std::uint32_t>(rhs.a);
                return Color{
                    static_cast<std::uint8_t>(dr > 255 ? static_cast<std::uint8_t>(dr - 255) : static_cast<std::uint8_t>(dr)),
                    static_cast<std::uint8_t>(dg > 255 ? static_cast<std::uint8_t>(dg - 255) : static_cast<std::uint8_t>(dg)),
                    static_cast<std::uint8_t>(db > 255 ? static_cast<std::uint8_t>(db - 255) : static_cast<std::uint8_t>(db)),
                    static_cast<std::uint8_t>(da > 255 ? static_cast<std::uint8_t>(da - 255) : static_cast<std::uint8_t>(da))
                };
            }
        }

        return lhs + rhs;
    }
    Color Color::subtract(const Color& lhs, const Color& rhs, const Mode mode /*= Mode::CLAMP*/)
    {
        switch(mode)
        {
            case Mode::Clamp:
            {
                return Color{
                    static_cast<std::uint8_t>(std::max(static_cast<std::int32_t>(lhs.r) - static_cast<std::int32_t>(rhs.r), 0)),
                    static_cast<std::uint8_t>(std::max(static_cast<std::int32_t>(lhs.g) - static_cast<std::int32_t>(rhs.g), 0)),
                    static_cast<std::uint8_t>(std::max(static_cast<std::int32_t>(lhs.b) - static_cast<std::int32_t>(rhs.b), 0)),
                    static_cast<std::uint8_t>(std::max(static_cast<std::int32_t>(lhs.a) - static_cast<std::int32_t>(rhs.a), 0))
                };
            }
            case Mode::Wrap:
            {
                const auto dr = static_cast<std::uint32_t>(lhs.r) - static_cast<std::uint32_t>(rhs.r);
                const auto dg = static_cast<std::uint32_t>(lhs.g) - static_cast<std::uint32_t>(rhs.g);
                const auto db = static_cast<std::uint32_t>(lhs.b) - static_cast<std::uint32_t>(rhs.b);
                const auto da = static_cast<std::uint32_t>(lhs.a) - static_cast<std::uint32_t>(rhs.a);
                return Color{
                    static_cast<std::uint8_t>(dr < 0 ? static_cast<std::uint8_t>(255 - dr) : static_cast<std::uint8_t>(dr)),
                    static_cast<std::uint8_t>(dg < 0 ? static_cast<std::uint8_t>(255 - dg) : static_cast<std::uint8_t>(dg)),
                    static_cast<std::uint8_t>(db < 0 ? static_cast<std::uint8_t>(255 - db) : static_cast<std::uint8_t>(db)),
                    static_cast<std::uint8_t>(da < 0 ? static_cast<std::uint8_t>(255 - da) : static_cast<std::uint8_t>(da))
                };
            }
        }

        return lhs - rhs;
    }
    Color Color::multiply(const Color& lhs, const Color& rhs, const Mode mode /*= Mode::CLAMP*/)
    {
        switch(mode)
        {
            case Mode::Clamp:
            {
                return clamp(static_cast<vec4f>(lhs) * static_cast<vec4f>(rhs));
            }
            case Mode::Wrap:
            {
                const auto dr = static_cast<std::uint32_t>(lhs.r) * static_cast<std::uint32_t>(rhs.r);
                const auto dg = static_cast<std::uint32_t>(lhs.g) * static_cast<std::uint32_t>(rhs.g);
                const auto db = static_cast<std::uint32_t>(lhs.b) * static_cast<std::uint32_t>(rhs.b);
                const auto da = static_cast<std::uint32_t>(lhs.a) * static_cast<std::uint32_t>(rhs.a);
                return Color{
                    static_cast<std::uint8_t>(dr % 256),
                    static_cast<std::uint8_t>(dg % 256),
                    static_cast<std::uint8_t>(db % 256),
                    static_cast<std::uint8_t>(da % 256)
                };
            }
        }
        return lhs * rhs;
    }
    Color Color::divide(const Color& lhs, const Color& rhs, const Mode mode /*= Mode::CLAMP*/)
    {
        switch(mode)
        {
            case Mode::Clamp:
            {
                return clamp(static_cast<vec4f>(lhs) / static_cast<vec4f>(rhs));
            }
            case Mode::Wrap:
            {
                const auto dr = static_cast<std::uint32_t>(lhs.r) / static_cast<std::uint32_t>(rhs.r);
                const auto dg = static_cast<std::uint32_t>(lhs.g) / static_cast<std::uint32_t>(rhs.g);
                const auto db = static_cast<std::uint32_t>(lhs.b) / static_cast<std::uint32_t>(rhs.b);
                const auto da = static_cast<std::uint32_t>(lhs.a) / static_cast<std::uint32_t>(rhs.a);
                return Color{
                    static_cast<std::uint8_t>(dr % 256),
                    static_cast<std::uint8_t>(dg % 256),
                    static_cast<std::uint8_t>(db % 256),
                    static_cast<std::uint8_t>(da % 256)
                };
            }
        }

        return lhs / rhs;
    }

    Color Color::lerp(const Color& start, const Color& end, const float t)
    {
        return start + (end - start) * t;
    }

    Color Color::random()
    {
        return Color{
            random::range(0, 255),
            random::range(0, 255),
            random::range(0, 255),
            random::range(0, 255)
        };
    }
    Color Color::random(Channel channel)
    {
        Color result{};
        if(channel & R) result.r = static_cast<std::uint8_t>(random::range(0, 255));
        if(channel & G) result.g = static_cast<std::uint8_t>(random::range(0, 255));
        if(channel & B) result.b = static_cast<std::uint8_t>(random::range(0, 255));
        if(channel & A) result.a = static_cast<std::uint8_t>(random::range(0, 255));

        return result;
    }

    Color Color::clamp(const vec4f vector)
    {
        return Color{
            std::min(std::max(vector.r, 0.0f), 1.0f),
            std::min(std::max(vector.g, 0.0f), 1.0f),
            std::min(std::max(vector.b, 0.0f), 1.0f),
            std::min(std::max(vector.a, 0.0f), 1.0f)
        };
    }
    #pragma endregion
}