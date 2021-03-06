#pragma once
#include <winuser.rh>

namespace pk
{
    class Keyboard
    {
        public:
        enum struct Key
        {
            Unknown = -1,
            A = 'A',
            B = 'B',
            C = 'C',
            D = 'D',
            E = 'E',
            F = 'F',
            G = 'G',
            H = 'H',
            I = 'I',
            J = 'J',
            K = 'K',
            L = 'L',
            M = 'M',
            N = 'N',
            O = 'O',
            P = 'P',
            Q = 'Q',
            R = 'R',
            S = 'S',
            T = 'T',
            U = 'U',
            V = 'V',
            W = 'W',
            X = 'X',
            Y = 'Y',
            Z = 'Z',
            Num0 = '0',
            Num1 = '1',
            Num2 = '2',
            Num3 = '3',
            Num4 = '4',
            Num5 = '5',
            Num6 = '6',
            Num7 = '7',
            Num8 = '8',
            Num9 = '9',
            Escape = VK_ESCAPE,
            LControl = VK_LCONTROL,
            LShift = VK_LSHIFT,
            LAlt = VK_LMENU,
            LSystem = VK_LWIN,
            RControl = VK_RCONTROL,
            RShift = VK_RSHIFT,
            RAlt = VK_RMENU,
            RSystem = VK_RWIN,
            Menu = VK_APPS,
            LBracket = VK_OEM_4,
            RBracket = VK_OEM_6,
            SemiColon = VK_OEM_1,
            Comma = VK_OEM_COMMA,
            Period = VK_OEM_PERIOD,
            Quote = VK_OEM_7,
            Slash = VK_OEM_2,
            BackSlash = VK_OEM_5,
            Tilde = VK_OEM_3,
            Equal = VK_OEM_PLUS,
            Dash = VK_OEM_MINUS,
            Space = VK_SPACE,
            Return = VK_RETURN,
            BackSpace = VK_BACK,
            Tab = VK_TAB,
            PageUp = VK_PRIOR,
            PageDown = VK_NEXT,
            End = VK_END,
            Home = VK_HOME,
            Insert = VK_INSERT,
            Delete = VK_DELETE,
            Add = VK_ADD,
            Subtract = VK_SUBTRACT,
            Multiply = VK_MULTIPLY,
            Divide = VK_DIVIDE,
            Left = VK_LEFT,
            Right = VK_RIGHT,
            Up = VK_UP,
            Down = VK_DOWN,
            Numpad0 = VK_NUMPAD0,
            Numpad1 = VK_NUMPAD1,
            Numpad2 = VK_NUMPAD2,
            Numpad3 = VK_NUMPAD3,
            Numpad4 = VK_NUMPAD4,
            Numpad5 = VK_NUMPAD5,
            Numpad6 = VK_NUMPAD6,
            Numpad7 = VK_NUMPAD7,
            Numpad8 = VK_NUMPAD8,
            Numpad9 = VK_NUMPAD9,
            F1 = VK_F1,
            F2 = VK_F2,
            F3 = VK_F3,
            F4 = VK_F4,
            F5 = VK_F5,
            F6 = VK_F6,
            F7 = VK_F7,
            F8 = VK_F8,
            F9 = VK_F9,
            F10 = VK_F10,
            F11 = VK_F11,
            F12 = VK_F12,
            F13 = VK_F13,
            F14 = VK_F14,
            F15 = VK_F15,
            Pause = VK_PAUSE,
            KeyCount
        };

        static bool isKeyPressed(const Keyboard::Key key);
        static bool isPhysicalKeyPressed(const Keyboard::Key key);
    };
}
