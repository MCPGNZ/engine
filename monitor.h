#pragma once
#include <vector>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "vec2.h"
#include "rect.h"

namespace pk
{
    class Monitor
    {
        private:
        struct Data
        {
            MONITORINFOEX info;
            recti virtual_rect;
        };
        std::vector<Data> monitors;
        vec2i stv_translation = {0};

        public:
        Monitor(void);

        vec2i screen_to_virtual(vec2i point);
        vec2i virtual_to_screen(vec2i point);

        vec2i virtual_resolution();

        private:
        static BOOL CALLBACK monitor_proc(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM data);
        void find_translation();
    };
}
