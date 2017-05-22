#include "Mouse.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "logger.h"
#include "window.h"

namespace pk
{
    bool Mouse::is_button_pressed(Button b)
    {
        int vkey = 0;
        switch(b)
        {
            case Button::left:     vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
            case Button::right:    vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
            case Button::middle:   vkey = VK_MBUTTON;  break;
            default:               vkey = 0;           break;
        }

        return (GetAsyncKeyState(vkey) & 0x8000) != 0;
    }

    vec2i Mouse::get_position()
    {
        POINT point;
        GetCursorPos(&point);

        return vec2i{point.x, point.y};
    }

    vec2i Mouse::get_position(const Window& window)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(window.getHandle(), &point);
        return vec2i{point.x, point.y};
    }

    void Mouse::set_position(const vec2i& position)
    {
        SetCursorPos(position.x, position.y);
    }

    void Mouse::set_position(const vec2i& position, const Window& w)
    {
        logger::error("Mouse", "set_position", "not implemented");
    }
}