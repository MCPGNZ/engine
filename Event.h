#pragma once
#include "vec2.h"
#include "keyboard.h"
#include "mouse.h"

namespace pk
{
    struct Event
    {
        enum struct Type
        {
            Undefined,
            Closed,
            Resized,
            FocusLost,
            FocusGained,
            KeyPressed,
            KeyReleased,
            AsciiInput,
            MouseButtonPressed,
            MouseButtonReleased,
            MouseWheelMoved,
            MouseLeft,
            MouseEntered,
            MouseMoved
        } type = Type::Undefined;

        vec2i size;
        Keyboard::Key key;

        struct
        {
            vec2i position;
            int delta;
        } mouseWheel;

        struct
        {
            int key;
        } ascii;

        struct
        {
            vec2i position;
            Mouse::Button button;
        } mouseButton;

        struct
        {
            vec2i position;
        } mouseMove;
    };
}
