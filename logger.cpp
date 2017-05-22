#include "logger.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace pk
{
    std::function<void(const std::string&)> logger::callback = nullptr;

    void logger::log(const std::string& message, type type /*= typeE::INFO */)
    {
        if(callback != nullptr)
        {
            callback(message);
        }

        int Color = 15;
        switch(type)
        {
            case type::LOG_ERROR:   Color = 12; break;
            case type::LOG_INFO:    Color = 7;  break;
            case type::LOG_WARNING: Color = 6;  break;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), static_cast<WORD>(Color));
        std::cout << message << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), 15);

        if(type == type::LOG_ERROR)
        {
            system("pause");
        }
    }
}