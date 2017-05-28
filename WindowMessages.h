#pragma once
#include "vec2.h"
#include "keyboard.h"
#include "mouse.h"
#include "Event.h"

#include "Filesystem.h"

namespace pk
{
    namespace messages
    {
        namespace window
        {
            struct closed {};
            struct resized { vec2i size; };

            struct keyDown { Keyboard::Key key; };
            struct keyUp { Keyboard::Key key; };

            struct dropped
            {
                const std::filesystem::path path;
            };

            struct mouseDown
            {
                Mouse::Button button;
                vec2i position;
            };
            struct mouseUp
            {
                Mouse::Button button;
                vec2i position;
            };

            struct ascii
            {
                unsigned short index;
            };

            struct mouseMoved { vec2i position; };
        }
    }
}