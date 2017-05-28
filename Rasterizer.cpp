#include "Rasterizer.h"

#include <d3d11.h>
#include "Renderer.h"


namespace pk
{
    void Rasterizer::create(bool scisorsEnabled /* = false */)
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.ScissorEnable = scisorsEnabled;
        desc.DepthClipEnable = true;
        Renderer::_device()->CreateRasterizerState(&desc, _state.GetAddressOf());
    }

    void Rasterizer::bind(Renderer& renderer)
    {
        Renderer::_context()->RSSetState(_state.Get());
    }
}