#include "SpriteSystemView.h"

#include <string>
#include "logger.h"
#include "SpriteSystem.h"
#include "UI.h"

#include "View.h"
#include "DragInfo.h"

namespace pk
{
    void SpriteSystemView::render(SpriteSystem& source)
    {
        if(gui::CollapsingHeader("sprite system"))
        {
            gui::Indent(8.0f);
            if(gui::CollapsingHeader("elements"))
            {
                if(gui::Button("add"))
                {
                    source.add();
                }
                for(int i = 0; i < source._sprites.size(); ++i)
                {
                    if(gui::Button(("sprite" + std::to_string(i)).c_str()))
                    {
                        View::_current->_function = [i, &source](){
                            auto view = SpriteView{i, source};
                            view.render();
                        };
                    }
                }
            }

            if(gui::CollapsingHeader("settings"))
            {
                gui::Object<Resources>("[resources]", &source._resources);

                gui::Object<Shader>("[vertex shader]", &source._vertexShader);
                gui::Object<Shader>("[pixel shader]", &source._pixelShader);
            }
        }
    }
}