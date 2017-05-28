#include "SpaceView.h"
#include "UI.h"
#include "Space.h"
#include "View.h"

#include "DragInfo.h"
#include "Resources.h"

namespace pk
{
    std::map<std::string, SpaceViewEntry> SpaceView::_entries;

    SpaceView::SpaceView(Space & scene) :
        _source(scene)
    {
        _name = "Space";
    }

    void SpaceView::render()
    {
        if(!_enabled)
        {
            return;
        }

        gui::Begin(_name.c_str());

        for(auto& name : _source._names)
        {
            if(gui::Button(name.c_str()))
            {
                _entries[name]._onClick();
            }
            if(_entries[name]._type == type_info<Resources>::type)
            {
                gui::Handle(pk::Resources::_components[0]);
            }
        }

        drawContext();
        gui::End();
    }

    void SpaceView::add(const std::string & menu, SpaceViewEntry entry)
    {
        _entries[menu] = entry;
    }

    void SpaceView::drawContext()
    {
        if(gui::BeginPopupContextWindow())
        {
            for(auto& entry : _entries)
            {
                if(gui::Button(entry.first.c_str()))
                {
                    View::_current->_enabled = true;
                    entry.second._menu(_source);
                }
            }
            gui::EndPopup();
        }
    }
}