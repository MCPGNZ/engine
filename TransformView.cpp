#include "TransformView.h"

#include "UI.h"

namespace pk
{
    void TransformView::render(Transform& source)
    {
        if(gui::CollapsingHeader("transform"))
        {
            gui::DragFloat3("position", source.position.v);
            gui::DragFloat3("rotation", source.rotation.v);
            gui::DragFloat3("scale", source.scale.v);
        }
    }
}