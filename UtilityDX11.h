#pragma once
#include <d3d11.h>
#include <string>
#include "GPUDesc.h"

namespace pk
{
    namespace dx
    {
        DXGI_FORMAT                    Convert(const TextureFormat format);
        D3D11_PRIMITIVE_TOPOLOGY       Convert(const Topology topology);
        D3D11_USAGE                    Convert(const BufferUsage usage);
        int                            Convert(const BufferBind bind);
        int                            Convert(const BufferCpuAccess access);

        LPCSTR GetSemanticName(ShaderLayout layout);

        DXGI_FORMAT ComponentToFormat(const D3D_REGISTER_COMPONENT_TYPE type, const int components, const std::string& semantic);
    }
}