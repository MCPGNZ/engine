#include "monitor.h"

namespace pk
{
    Monitor::Monitor(void)
    {
        EnumDisplayMonitors(NULL, NULL, monitor_proc, (LPARAM)this);
        find_translation();
    }

    vec2i Monitor::screen_to_virtual(vec2i point)
    {
        return point - stv_translation;
    }

    vec2i Monitor::virtual_to_screen(vec2i point)
    {
        return point + stv_translation;
    }

    vec2i Monitor::virtual_resolution()
    {
        return vec2i{
            GetSystemMetrics(SM_CXVIRTUALSCREEN),
            GetSystemMetrics(SM_CYVIRTUALSCREEN)
        };
    }

    BOOL CALLBACK Monitor::monitor_proc(HMONITOR monitor, HDC dc, LPRECT rect, LPARAM self)
    {
        Monitor* _this = reinterpret_cast<Monitor*>(self);

        MONITORINFOEX info;
        info.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(monitor, &info);

        Data data{};
        data.info = info;

        _this->monitors.push_back(data);

        return TRUE;
    }

    void Monitor::find_translation()
    {
        if(monitors.size() == 1)
        {
            return;
        }

        stv_translation = vec2i::maximum();
        for(const auto& m : monitors)
        {
            const auto& r = m.info.rcMonitor;
            if(r.left < stv_translation.x) { stv_translation.x = r.left; }
            if(r.top < stv_translation.y) { stv_translation.y = r.top; }
        }
    }
}