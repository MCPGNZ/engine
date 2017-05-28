#include "Window.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>
#include <shellapi.h>

#include "logger.h"

#include "MessageDispatcher.h"
#include "WindowMessages.h"

namespace pk
{
    Window::Window(const vec2i s, const std::string& title /* = "pk:: */)
    {
        create(s, title);
    }
    Window::~Window(void)
    {
        destroy();
    }

    void Window::create(const vec2i s, const std::string& title /*= "pk::"*/)
    {
        if(isOpenFlag)
        {
            destroy();
        }

        registerWindowClass();

        HDC dc = GetDC(NULL);
        vec2i screenRes = {GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES)};
        ReleaseDC(NULL, dc);

        vec2i position = (screenRes - s) / 2;

        handle = CreateWindow("pk_Window_class", title.c_str(), WS_SYSMENU | WS_THICKFRAME,
            position.x, position.y, s.width, s.height, NULL, NULL, GetModuleHandle(NULL), this);

        size(s);
        ShowWindow(handle, true);
        setMouseVisible(true);

        DragAcceptFiles(handle, true);

        isOpenFlag = true;
    }
    void Window::destroy()
    {
        if(icon)
        {
            DestroyIcon(icon);
        }

        if(handle)
        {
            DestroyWindow(handle);
        }
    }

    #pragma region Public methods
    void Window::broadcastEvents()
    {
        pk::Event event;
        while(pollEvent(event)) {}
    }

    bool Window::isOpen() const
    {
        return isOpenFlag;
    }

    HWND Window::getHandle() const
    {
        return handle;
    }
    vec2i Window::size() const
    {
        vec2i result;

        RECT rect;
        GetClientRect(handle, &rect);

        result.x = rect.right - rect.left;
        result.y = rect.bottom - rect.top;

        return result;
    }

    vec2i Window::position() const
    {
        vec2i result;

        RECT rect;
        GetWindowRect(handle, &rect);

        result.x = rect.left;
        result.y = rect.top;

        return result;
    }
    void Window::position(const vec2i position)
    {
        SetWindowPos(handle, NULL, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    std::string Window::title() const
    {
        std::string result;
        result.resize(512);

        GetWindowText(handle, &result.front(), 512);

        for(int i = 0; i < static_cast<int>(result.size()); ++i)
        {
            if(result[i] == '\0')
            {
                result.erase(i, result.size() - 1);
                break;
            }
        }

        return result;
    }
    bool Window::hasFocus() const
    {
        return handle == GetForegroundWindow();
    }

    void Window::size(const vec2i size)
    {
        RECT rectangle = {0, 0, static_cast<long>(size.x), static_cast<long>(size.y)};
        AdjustWindowRect(&rectangle, GetWindowLong(handle, GWL_STYLE), false);

        const vec2i computedSize = {
            rectangle.right - rectangle.left,
            rectangle.bottom - rectangle.top
        };

        SetWindowPos(handle, NULL, 0, 0, computedSize.x, computedSize.y, SWP_NOMOVE | SWP_NOZORDER);
    }

    void Window::title(const std::string& title)
    {
        SetWindowText(handle, title.c_str());
    }
    void Window::visible(const bool state)
    {
        ShowWindow(handle, state ? SW_SHOW : SW_HIDE);
    }
    void Window::setMouseVisible(const bool state)
    {
        cursor = state ? LoadCursor(NULL, IDC_ARROW) : nullptr;
        SetCursor(cursor);
    }
    void Window::focus()
    {
        SetForegroundWindow(handle);
    }

    void Window::center()
    {
        RECT rc;

        GetWindowRect(handle, &rc);

        int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
        int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

        SetWindowPos(handle, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    float Window::aspect() const
    {
        auto s = size();
        return (float) s.y / (float) s.x;
    }
    #pragma endregion

    #pragma region Private methods
    bool Window::pollEvent(Event& event)
    {
        if(Events.empty())
        {
            handleEvents();
        }

        if(isOpenFlag)
        {
            if(!Events.empty())
            {
                event = Events.front();
                Events.pop();

                return true;
            }
        }

        return false;
    }

    void Window::handleEvents()
    {
        MSG message;
        while(PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    void Window::registerWindowClass()
    {
        LPSTR className = "pk_Window_class";

        WNDCLASS wc;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &Window::wndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = 0;
        wc.hCursor = 0;
        wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = className;

        RegisterClass(&wc);
    }

    LRESULT CALLBACK Window::wndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        Window* w = handle ? reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

        if(msg == WM_CREATE)
        {
            LONG_PTR w = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams;
            SetWindowLongPtr(handle, GWLP_USERDATA, w);
        }

        if(w)
        {
            w->processEvent(msg, wparam, lparam);
        }

        if(msg == WM_CLOSE || ((msg == WM_SYSCOMMAND) && (msg == SC_KEYMENU)))
            return 0;

        return DefWindowProc(handle, msg, wparam, lparam);
    }

    void Window::processEvent(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        using namespace messages::window;

        Event e;
        switch(msg)
        {
            case WM_DESTROY:   break;
            case WM_SETCURSOR:
            {
                if(LOWORD(lparam) == HTCLIENT) SetCursor(cursor);
                break;
            }
            case WM_CLOSE:
            {
                e.type = Event::Type::Closed;
                MessageDispatcher::Signal(closed{}, this);
                break;
            }
            case WM_SIZE:
            {
                e.type = Event::Type::Resized;
                e.size = size();

                MessageDispatcher::Signal(resized{size()}, this);
                break;
            }
            case WM_ENTERSIZEMOVE:
            {
                resizing = true;
                break;
            }
            case WM_EXITSIZEMOVE:
            {
                resizing = false;
                break;
            }
            case WM_SETFOCUS:
            {
                e.type = Event::Type::FocusGained;
                break;
            }
            case WM_KILLFOCUS:
            {
                e.type = Event::Type::FocusLost;
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
                e.type = Event::Type::KeyPressed;
                e.key = keyFromWin32(wparam, lparam);
                MessageDispatcher::Signal(keyDown{e.key}, this);
                break;
            }
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                e.type = Event::Type::KeyReleased;
                e.key = keyFromWin32(wparam, lparam);
                MessageDispatcher::Signal(keyUp{e.key}, this);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                POINT position;
                position.x = GET_X_LPARAM(lparam);
                position.y = GET_Y_LPARAM(lparam);
                ScreenToClient(handle, &position);

                e.type = Event::Type::MouseWheelMoved;
                e.mouseWheel.position.x = position.x;
                e.mouseWheel.position.y = position.y;
                e.mouseWheel.delta = GET_Y_LPARAM(wparam) / WHEEL_DELTA;
                break;
            }
            case WM_LBUTTONDOWN:
            {
                e.type = Event::Type::MouseButtonPressed;
                e.mouseButton.button = Mouse::Button::left;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);

                MessageDispatcher::Signal(mouseDown{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_LBUTTONUP:
            {
                e.type = Event::Type::MouseButtonReleased;
                e.mouseButton.button = Mouse::Button::left;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);
                MessageDispatcher::Signal(mouseUp{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_RBUTTONDOWN:
            {
                e.type = Event::Type::MouseButtonPressed;
                e.mouseButton.button = Mouse::Button::right;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);

                MessageDispatcher::Signal(mouseDown{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_RBUTTONUP:
            {
                e.type = Event::Type::MouseButtonReleased;
                e.mouseButton.button = Mouse::Button::right;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);

                MessageDispatcher::Signal(mouseUp{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_MBUTTONDOWN:
            {
                e.type = Event::Type::MouseButtonPressed;
                e.mouseButton.button = Mouse::Button::middle;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);
                MessageDispatcher::Signal(mouseDown{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_MBUTTONUP:
            {
                e.type = Event::Type::MouseButtonReleased;
                e.mouseButton.button = Mouse::Button::middle;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);

                MessageDispatcher::Signal(mouseUp{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_XBUTTONDOWN:
            {
                e.type = Event::Type::MouseButtonPressed;
                e.mouseButton.button = HIWORD(wparam) == XBUTTON1 ? Mouse::Button::X1 : Mouse::Button::X2;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);

                MessageDispatcher::Signal(mouseDown{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_XBUTTONUP:
            {
                e.type = Event::Type::MouseButtonReleased;
                e.mouseButton.button = HIWORD(wparam) == XBUTTON1 ? Mouse::Button::X1 : Mouse::Button::X2;
                e.mouseButton.position.x = GET_X_LPARAM(lparam);
                e.mouseButton.position.y = GET_Y_LPARAM(lparam);

                MessageDispatcher::Signal(mouseUp{
                    e.mouseButton.button,
                    e.mouseButton.position
                }, this);
                break;
            }
            case WM_MOUSELEAVE:
            {
                e.type = Event::Type::MouseLeft;
                break;
            }
            case WM_MOUSEMOVE:
            {
                vec2i position;
                position.x = GET_X_LPARAM(lparam);
                position.y = GET_Y_LPARAM(lparam);

                e.type = Event::Type::MouseMoved;
                e.mouseMove.position = position;
                MessageDispatcher::Signal(mouseMoved{e.mouseMove.position}, this);
                break;
            }
            case WM_CHAR:
            {
                if(wparam > 0 && wparam < 0x10000)
                {
                    e.type = Event::Type::AsciiInput;
                    e.ascii.key = wparam;
                    MessageDispatcher::Signal(ascii{(unsigned short) wparam}, this);
                }
                break;
            }
            case WM_DROPFILES:
            {
                TCHAR lpszFile[MAX_PATH] = {0};
                UINT fileCount = 0;
                HDROP hDrop = (HDROP) wparam;

                fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);

                for(int i = 0; i < fileCount; ++i)
                {
                    lpszFile[0] = '\0';
                    if(DragQueryFile(hDrop, i, lpszFile, MAX_PATH))
                    {
                        MessageDispatcher::Signal(dropped{std::string{lpszFile}}, this);
                    }
                }

                DragFinish(hDrop);
                break;
            }
        }

        if(e.type != Event::Type::Undefined)
        {
            Events.push(e);
        }
    }

    Keyboard::Key Window::keyFromWin32(WPARAM key, LPARAM flags)
    {
        switch(key)
        {
            case VK_SHIFT:
            {
                static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
                UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
                return scancode == lShift ? Keyboard::Key::LShift : Keyboard::Key::RShift;
            }
            case VK_MENU:       return (HIWORD(flags) & KF_EXTENDED) ? Keyboard::Key::RAlt : Keyboard::Key::LAlt;
            case VK_CONTROL:    return (HIWORD(flags) & KF_EXTENDED) ? Keyboard::Key::RControl : Keyboard::Key::LControl;
            case VK_LWIN:       return Keyboard::Key::LSystem;
            case VK_RWIN:       return Keyboard::Key::RSystem;
            case VK_APPS:       return Keyboard::Key::Menu;
            case VK_OEM_1:      return Keyboard::Key::SemiColon;
            case VK_OEM_2:      return Keyboard::Key::Slash;
            case VK_OEM_PLUS:   return Keyboard::Key::Equal;
            case VK_OEM_MINUS:  return Keyboard::Key::Dash;
            case VK_OEM_4:      return Keyboard::Key::LBracket;
            case VK_OEM_6:      return Keyboard::Key::RBracket;
            case VK_OEM_COMMA:  return Keyboard::Key::Comma;
            case VK_OEM_PERIOD: return Keyboard::Key::Period;
            case VK_OEM_7:      return Keyboard::Key::Quote;
            case VK_OEM_5:      return Keyboard::Key::BackSlash;
            case VK_OEM_3:      return Keyboard::Key::Tilde;
            case VK_ESCAPE:     return Keyboard::Key::Escape;
            case VK_SPACE:      return Keyboard::Key::Space;
            case VK_RETURN:     return Keyboard::Key::Return;
            case VK_BACK:       return Keyboard::Key::BackSpace;
            case VK_TAB:        return Keyboard::Key::Tab;
            case VK_PRIOR:      return Keyboard::Key::PageUp;
            case VK_NEXT:       return Keyboard::Key::PageDown;
            case VK_END:        return Keyboard::Key::End;
            case VK_HOME:       return Keyboard::Key::Home;
            case VK_INSERT:     return Keyboard::Key::Insert;
            case VK_DELETE:     return Keyboard::Key::Delete;
            case VK_ADD:        return Keyboard::Key::Add;
            case VK_SUBTRACT:   return Keyboard::Key::Subtract;
            case VK_MULTIPLY:   return Keyboard::Key::Multiply;
            case VK_DIVIDE:     return Keyboard::Key::Divide;
            case VK_PAUSE:      return Keyboard::Key::Pause;
            case VK_F1:         return Keyboard::Key::F1;
            case VK_F2:         return Keyboard::Key::F2;
            case VK_F3:         return Keyboard::Key::F3;
            case VK_F4:         return Keyboard::Key::F4;
            case VK_F5:         return Keyboard::Key::F5;
            case VK_F6:         return Keyboard::Key::F6;
            case VK_F7:         return Keyboard::Key::F7;
            case VK_F8:         return Keyboard::Key::F8;
            case VK_F9:         return Keyboard::Key::F9;
            case VK_F10:        return Keyboard::Key::F10;
            case VK_F11:        return Keyboard::Key::F11;
            case VK_F12:        return Keyboard::Key::F12;
            case VK_F13:        return Keyboard::Key::F13;
            case VK_F14:        return Keyboard::Key::F14;
            case VK_F15:        return Keyboard::Key::F15;
            case VK_LEFT:       return Keyboard::Key::Left;
            case VK_RIGHT:      return Keyboard::Key::Right;
            case VK_UP:         return Keyboard::Key::Up;
            case VK_DOWN:       return Keyboard::Key::Down;
            case VK_NUMPAD0:    return Keyboard::Key::Numpad0;
            case VK_NUMPAD1:    return Keyboard::Key::Numpad1;
            case VK_NUMPAD2:    return Keyboard::Key::Numpad2;
            case VK_NUMPAD3:    return Keyboard::Key::Numpad3;
            case VK_NUMPAD4:    return Keyboard::Key::Numpad4;
            case VK_NUMPAD5:    return Keyboard::Key::Numpad5;
            case VK_NUMPAD6:    return Keyboard::Key::Numpad6;
            case VK_NUMPAD7:    return Keyboard::Key::Numpad7;
            case VK_NUMPAD8:    return Keyboard::Key::Numpad8;
            case VK_NUMPAD9:    return Keyboard::Key::Numpad9;
            case 'A':           return Keyboard::Key::A;
            case 'Z':           return Keyboard::Key::Z;
            case 'E':           return Keyboard::Key::E;
            case 'R':           return Keyboard::Key::R;
            case 'T':           return Keyboard::Key::T;
            case 'Y':           return Keyboard::Key::Y;
            case 'U':           return Keyboard::Key::U;
            case 'I':           return Keyboard::Key::I;
            case 'O':           return Keyboard::Key::O;
            case 'P':           return Keyboard::Key::P;
            case 'Q':           return Keyboard::Key::Q;
            case 'S':           return Keyboard::Key::S;
            case 'D':           return Keyboard::Key::D;
            case 'F':           return Keyboard::Key::F;
            case 'G':           return Keyboard::Key::G;
            case 'H':           return Keyboard::Key::H;
            case 'J':           return Keyboard::Key::J;
            case 'K':           return Keyboard::Key::K;
            case 'L':           return Keyboard::Key::L;
            case 'M':           return Keyboard::Key::M;
            case 'W':           return Keyboard::Key::W;
            case 'X':           return Keyboard::Key::X;
            case 'C':           return Keyboard::Key::C;
            case 'V':           return Keyboard::Key::V;
            case 'B':           return Keyboard::Key::B;
            case 'N':           return Keyboard::Key::N;
            case '0':           return Keyboard::Key::Num0;
            case '1':           return Keyboard::Key::Num1;
            case '2':           return Keyboard::Key::Num2;
            case '3':           return Keyboard::Key::Num3;
            case '4':           return Keyboard::Key::Num4;
            case '5':           return Keyboard::Key::Num5;
            case '6':           return Keyboard::Key::Num6;
            case '7':           return Keyboard::Key::Num7;
            case '8':           return Keyboard::Key::Num8;
            case '9':           return Keyboard::Key::Num9;
        }
        return Keyboard::Key::Unknown;
    }
    #pragma endregion
}