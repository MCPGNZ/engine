#pragma once
#include "WindowView.h"


namespace pk
{
    class Console;

    class ConsoleView : WindowView
    {
    public:
        ConsoleView(Console& console);
        void render() override;

    private:
        Console& _source;
    };
}
