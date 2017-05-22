#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#undef NOMINMAX

using window_handle = HWND;
