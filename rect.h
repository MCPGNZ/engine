#pragma once
#include <cstdint>

namespace pk
{
    template <typename T>
    class rect
    {
        public:
        union
        {
            T v[4];
            struct
            {
                T x;
                T y;
                T width;
                T height;
            };
        };

        public:
        rect(void) = default;

        template <typename U>
        bool inside(const vec2<U>& point);
    };

    template <typename T>
    template <typename U>
    bool rect<T>::inside(const vec2<U>& point)
    {
        return (point.x >= x) &&
            (point.x <= x + width) &&
            (point.y >= y) &&
            (point.y <= y + height);
    }

    using rectb = rect<uint8_t>;
    using recti = rect<int32_t>;
    using rectu = rect<uint32_t>;
    using rectf = rect<float>;
    using rectd = rect<double>;
}