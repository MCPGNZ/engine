#include "DepthState.h"
#include "Renderer.h"
#include "logger.h"

namespace pk
{
    void DepthState::create(bool depthEnable, bool stencilEnable /* = false */)
    {
        _depthStencilState = nullptr;

        const auto& device = Renderer::_device();

        D3D11_DEPTH_STENCIL_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        desc.DepthEnable = depthEnable;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS;
        desc.StencilEnable = stencilEnable;
        desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.BackFace = desc.FrontFace;

        auto hr = device->CreateDepthStencilState(&desc, _depthStencilState.GetAddressOf());
        logger::assert_hr(hr, "RenderTarget", "create_depth_stencil_state", "CreateDepthStencilState(WRITE_ENABLE)");
    }
    void DepthState::bind(Renderer& renderer)
    {
        Renderer::_context()->OMSetDepthStencilState(_depthStencilState.Get(), 0);
    }
}