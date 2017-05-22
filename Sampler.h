#pragma once
#include <wrl\client.h>
#include <d3d11.h>

namespace pk
{
    #pragma region Usings
    using Microsoft::WRL::ComPtr;
    #pragma endregion

    class Sampler
    {
    public:
        Sampler(void);
        void create();

    private:
        ComPtr<ID3D11SamplerState> _sampler;

        friend class Renderer;
    };
}
