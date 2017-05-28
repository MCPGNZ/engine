#pragma once
#include <string>
#include <queue>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "vec2.h"
#include "traits.h"

#include "Event.h"
#include "window_handle.h"

namespace pk
{
    class Window
    {
    public:
        #pragma region Ctors
        Window(void) = default;
        Window(const vec2i size, const std::string& title = "pk::");
        #pragma endregion

        ~Window(void);

        #pragma region Public methods
        template <typename F,
            typename = IS_TRUE_VALUE(function_traits<F>::parameter_count == 1),
            typename = IS_SAME_TYPE(function_traits<F>::parameter<0>, const pk::Event&),
            typename = IS_SAME_TYPE(function_traits<F>::return_type, void)
        >
            void handleEvents(const F& func)
        {
            pk::Event event;
            while(pollEvent(event))
            {
                func(event);
            }
        }

        void broadcastEvents();

        bool isOpen() const;

        void create(const vec2i size, const std::string& title = "pk::");
        void destroy();

        vec2i position() const;
        void position(const vec2i position);

        std::string title() const;
        void title(const std::string& title);

        vec2i size() const;
        void size(const vec2i s);

        bool hasFocus() const;
        void focus();

        float aspect() const;
        void center();

        HWND getHandle() const;

        void visible(const bool state);
        void setMouseVisible(const bool state);
        #pragma endregion

    private:
        #pragma region Private variables
        HWND    handle = nullptr;
        HCURSOR cursor = nullptr;
        HICON   icon = nullptr;
        bool resizing = false;

        std::queue<Event> Events;

        bool isOpenFlag = false;
        bool fullscreen = false;
        #pragma endregion

        #pragma region Private methods
        bool pollEvent(Event& event);

        void handleEvents();

        void registerWindowClass();
        static LRESULT CALLBACK wndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        void processEvent(UINT msg, WPARAM wparam, LPARAM lparam);
        Keyboard::Key keyFromWin32(WPARAM key, LPARAM flags);
        #pragma endregion
    };
}
