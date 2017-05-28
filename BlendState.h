#pragma once

#include <wrl\client.h>
#include <d3d11.h>
#include "GPUDesc.h"

namespace pk
{
    using Microsoft::WRL::ComPtr;

    class BlendState
    {
    public:
        BlendState(void) = default;
        BlendState(const Blend type);

        void create(const Blend type);

    private:
        ComPtr<ID3D11BlendState> _state = nullptr;
        friend class Renderer;
    };
}
