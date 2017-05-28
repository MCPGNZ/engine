#include <string>

#include "SpriteView.h"

#include "Sprite.h"
#include "SpriteSystem.h"

#include "UI.h"
#include "TransformView.h"

namespace pk
{
    SpriteView::SpriteView(int component, SpriteSystem& system) :
        _component{component}, _system{&system}
    {}

    void SpriteView::render()
    {
        if(_system == nullptr)
        {
            return;
        }

        Sprite& sprite = _system->sprite(_component);
        Transform& transform = _system->transform(_component);

        TransformView::render(transform);

        if(gui::CollapsingHeader("sprite"))
        {
            gui::InputInt("texture id: ", &sprite._texture);
        }
    }
}