#include "View.h"

#include "UI.h"

namespace pk
{
    View* View::_current = nullptr;
   
    View::View(void)
    {
        _current = this;
        _name = "view";
    }

    void View::render()
    {
        if(_function == nullptr)
        {
            return;
        }

        gui::Begin("View");
        _function();
        gui::End();
    }
}