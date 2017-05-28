#pragma once
#include <vector>
#include <string>

namespace pk
{
    class WindowView
    {
    public:
        WindowView(void);
        virtual ~WindowView(void);

        std::string _name;
        bool _enabled = false;

        virtual void render() = 0;

        static std::vector<WindowView*> _views;
    };
}
