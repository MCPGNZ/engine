#include "Viewport.h"

#include "Renderer.h"
#include "RenderTarget.h"

namespace pk
{
    #pragma region Ctors
    Viewport::Viewport(const vec2f& _position, const vec2f& _scale) :
        position{_position}, scale{_scale}
    {}
    #pragma endregion
}