#pragma once
#include <d3d11.h>
#include <wrl\client.h>

namespace pk
{
    using namespace ::Microsoft::WRL;
    class Renderer;

    class Rasterizer
    {
    public:
        void create(bool scisorsEnabled = false);
        void bind(Renderer& renderer);

    private:
        ComPtr<ID3D11RasterizerState> _state = nullptr;
    };
}
