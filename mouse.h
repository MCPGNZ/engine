#pragma once
#include "vec2.h"

namespace pk
{
    class Window;

    struct Mouse
    {
        enum struct Button
        {
            left,
            right,
            middle,
            X1,
            X2
        };

        static bool is_button_pressed(Button b);

        static vec2i get_position();
        static vec2i get_position(const Window& window);

        static void set_position(const vec2i& position);
        static void set_position(const vec2i& position, const Window& w);
    };
}
