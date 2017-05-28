#include "ConsoleView.h"
#include "UI.h"

#include "Console.h"

namespace pk
{
    ConsoleView::ConsoleView(Console& console) :
        _source{console}
    {
        _name = "Console";
    }

    void ConsoleView::render()
    {
        gui::Begin(_name.c_str());

        for(auto& entry : _source._commands)
        {
            if(gui::Button(entry.first.c_str()))
            {
                entry.second();
            }
        }

        gui::End();
    }
}