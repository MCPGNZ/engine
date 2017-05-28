#include "ResourcesView.h"
#include "Resources.h"

#include "UI.h"
#include "View.h"
#include "stl_extentions.h"

namespace pk
{
    void pk::ResourcesView::render(Resources& source)
    {
        save(source);
        items(source);
    }

    void ResourcesView::save(Resources& source)
    {
        static Mode mode;

        if(gui::Button("Save"))
        {
            gui::OpenPopup("Popup");
            mode = Mode::Save;
        }
        gui::SameLine();
        if(gui::Button("Load"))
        {
            gui::OpenPopup("Popup");
            mode = Mode::Load;
        }

        if(gui::BeginPopupModal("Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char buffer[256] = "";
            if(gui::InputText("path", buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue) || gui::Button("save"))
            {
                switch(mode)
                {
                    case Mode::Load: source.load(std::string{buffer}); break;
                    case Mode::Save: source.save(std::string{buffer}); break;
                }

                gui::CloseCurrentPopup();
            }

            gui::SameLine();
            if(gui::Button("cancel"))
            {
                gui::CloseCurrentPopup();
            }

            gui::EndPopup();
        }
    }

    void ResourcesView::items(Resources& source)
    {
        for(auto& texture : source._textures)
        {
        }
    }
}