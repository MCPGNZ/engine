#pragma once
#include <wrl/client.h>
#include <d3d11.h>

namespace pk
{
    using namespace ::Microsoft::WRL;
    class Renderer;

    class DepthState
    {
    public:
        void create(bool depthEnable, bool stencilEnable = false);

        void bind(Renderer& renderer);

    private:
        ComPtr<ID3D11Texture2D>         _depthStencil = nullptr;
        ComPtr<ID3D11DepthStencilView>  _depthStencilView = nullptr;
        ComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
    };
}
