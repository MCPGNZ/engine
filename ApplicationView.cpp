#include "ApplicationView.h"

#include "UI.h"
#include "WindowView.h"

namespace pk
{
    void ApplicationView::render()
    {
        gui::BeginMainMenuBar();
        if(gui::BeginMenu("Engine"))
        {
            gui::EndMenu();
        }
        if(gui::BeginMenu("Create"))
        {
            if(gui::Button("Space"))
            {
            }
            gui::EndMenu();
        }
        if(gui::BeginMenu("Windows"))
        {
            for(auto& view : WindowView::_views)
            {
                if(gui::Button(view->_name.c_str()))
                {
                    view->_enabled = !view->_enabled;
                }
            }

            gui::EndMenu();
        }
        gui::EndMainMenuBar();

        for(auto& view : WindowView::_views)
        {
            if(view->_enabled)
            {
                view->render();
            }
        }
    }
}