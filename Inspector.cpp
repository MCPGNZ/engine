#include "Inspector.h"
#include "UI.h"


namespace pk
{
    Inspector::Inspector(void)
    {
        
    }

    void Inspector::render()
    {
        gui::Begin("Inspector");

        if(_selected != nullptr)
        {
            _selected(_data);
        }

        gui::End();
    }
}