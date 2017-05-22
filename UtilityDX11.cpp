#include "UtilityDX11.h"
#include "logger.h"
#include <unordered_map>

namespace pk
{
    namespace dx
    {
        DXGI_FORMAT Convert(const TextureFormat format)
        {
            switch(format)
            {
                case TextureFormat::Unknown:        return DXGI_FORMAT_UNKNOWN;
                case TextureFormat::Rgba32Typeless: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
                case TextureFormat::Rgba32Float:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
                case TextureFormat::Rgba32Uint:     return DXGI_FORMAT_R32G32B32A32_UINT;
                case TextureFormat::Rgba32Int:      return DXGI_FORMAT_R32G32B32A32_SINT;

                case TextureFormat::Rgb32Typeless:  return DXGI_FORMAT_R32G32B32_TYPELESS;
                case TextureFormat::Rgb32Float:     return DXGI_FORMAT_R32G32B32_FLOAT;
                case TextureFormat::Rgb32Uint:      return DXGI_FORMAT_R32G32B32_UINT;
                case TextureFormat::Rgb32Int:       return DXGI_FORMAT_R32G32B32_SINT;

                case TextureFormat::Rg32Typeless:   return DXGI_FORMAT_R32G32_TYPELESS;
                case TextureFormat::Rg32Float:      return DXGI_FORMAT_R32G32_FLOAT;
                case TextureFormat::Rg32Uint:       return DXGI_FORMAT_R32G32_UINT;
                case TextureFormat::Rg32Int:        return DXGI_FORMAT_R32G32_SINT;

                case TextureFormat::R32Typeless:    return DXGI_FORMAT_R32_TYPELESS;
                case TextureFormat::R32Float:       return DXGI_FORMAT_R32_FLOAT;
                case TextureFormat::R32Uint:        return DXGI_FORMAT_R32_UINT;
                case TextureFormat::R32Int:         return DXGI_FORMAT_R32_SINT;

                case TextureFormat::D32Float:       return DXGI_FORMAT_D32_FLOAT;

                    //16 bits per channel
                case TextureFormat::Rgba16Typeless: return DXGI_FORMAT_R16G16B16A16_TYPELESS;
                case TextureFormat::Rgba16Float:    return DXGI_FORMAT_R16G16B16A16_FLOAT;
                case TextureFormat::Rgba16Uint:     return DXGI_FORMAT_R16G16B16A16_UINT;
                case TextureFormat::Rgba16Int:      return DXGI_FORMAT_R16G16B16A16_SINT;

                case TextureFormat::Rg16Typeless:   return DXGI_FORMAT_R16G16_TYPELESS;
                case TextureFormat::Rg16Float:      return DXGI_FORMAT_R16G16_FLOAT;
                case TextureFormat::Rg16Uint:       return DXGI_FORMAT_R16G16_UINT;
                case TextureFormat::Rg16Int:        return DXGI_FORMAT_R16G16_SINT;
                case TextureFormat::Rg16Unorm:      return DXGI_FORMAT_R16G16_UNORM;

                case TextureFormat::R16Typeless:    return DXGI_FORMAT_R16_TYPELESS;
                case TextureFormat::R16Float:       return DXGI_FORMAT_R16_FLOAT;
                case TextureFormat::R16Uint:        return DXGI_FORMAT_R16_UINT;
                case TextureFormat::R16Int:         return DXGI_FORMAT_R16_SINT;
                case TextureFormat::R16Unorm:       return DXGI_FORMAT_R16_UNORM;

                    //8 bits per channel
                case TextureFormat::Rgba8Typeless: return DXGI_FORMAT_R8G8B8A8_TYPELESS;
                case TextureFormat::Rgba8Uint:     return DXGI_FORMAT_R8G8B8A8_UINT;
                case TextureFormat::Rgba8Int:      return DXGI_FORMAT_R8G8B8A8_SINT;
                case TextureFormat::Rgba8Unorm:    return DXGI_FORMAT_R8G8B8A8_UNORM;

                case TextureFormat::R8Typeless:    return DXGI_FORMAT_R8_TYPELESS;
                case TextureFormat::R8Uint:        return DXGI_FORMAT_R8_UINT;
                case TextureFormat::R8Int:         return DXGI_FORMAT_R8_SINT;
                case TextureFormat::R8Unorm:       return DXGI_FORMAT_R8_UNORM;

                default:
                //logger::error("Convert", "TextureFormat", "Format not found");
                return static_cast<DXGI_FORMAT>(0);
            }
        }
        D3D11_PRIMITIVE_TOPOLOGY Convert(const Topology topology)
        {
            switch(topology)
            {
                case Topology::Undefined:         return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
                case Topology::LineList:         return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
                case Topology::LineStrip:        return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
                case Topology::PointList:        return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
                case Topology::TriangleList:     return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case Topology::TriangleStrip:    return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                default:
                //logger::error("Convert", "Topology", "Format not found");
                return static_cast<D3D11_PRIMITIVE_TOPOLOGY>(0);
            }
        }
        D3D11_USAGE Convert(const BufferUsage usage)
        {
            switch(usage)
            {
                case BufferUsage::Static:  return D3D11_USAGE_DEFAULT;
                case BufferUsage::Dynamic: return D3D11_USAGE_DYNAMIC;
                case BufferUsage::Staging: return D3D11_USAGE_STAGING;

                default:
                //logger::error("Convert", "BUFFER_USAGE", "Format not found");
                return static_cast<D3D11_USAGE>(0);
            }
        }
        int Convert(const BufferBind bind)
        {
            switch(bind)
            {
                case BufferBind::Vertex:         return D3D11_BIND_VERTEX_BUFFER;
                case BufferBind::Index:          return D3D11_BIND_INDEX_BUFFER;
                case BufferBind::ConstantBuffer: return D3D11_BIND_CONSTANT_BUFFER;

                default:
                //logger::error("Convert", "BUFFER_BIND", "Format not found");
                return 0;
            }
        }
        int Convert(const BufferCpuAccess access)
        {
            switch(access)
            {
                case BufferCpuAccess::None:      return 0;
                case BufferCpuAccess::Read:      return D3D11_CPU_ACCESS_READ;
                case BufferCpuAccess::Write:     return D3D11_CPU_ACCESS_WRITE;
                case BufferCpuAccess::ReadWrite: return D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                default:
                logger::error("Convert", "BUFFER_GPU_ACCESS", "Format not found");
                return 0;
            }
        }

        LPCSTR GetSemanticName(ShaderLayout layout)
        {
            static std::unordered_map<ShaderLayout, LPCSTR> map
            {
                {ShaderLayout::Position, "POSITION"},
                {ShaderLayout::Texcoord, "TEXCOORD"},
                {ShaderLayout::Color, "COLOR"},
            };

            return map[layout];
        }

        DXGI_FORMAT ComponentToFormat(const D3D_REGISTER_COMPONENT_TYPE type, const int components, const std::string& semantic = "")
        {
            switch(components)
            {
                case 1:
                {
                    if(semantic == "COLOR") return DXGI_FORMAT_R8_UNORM;
                    switch(type)
                    {
                        case D3D_REGISTER_COMPONENT_UINT32:  return DXGI_FORMAT_R32_UINT;
                        case D3D_REGISTER_COMPONENT_SINT32:  return DXGI_FORMAT_R32_SINT;
                        case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32_FLOAT;
                    }
                }
                case 2:
                {
                    if(semantic == "COLOR") return DXGI_FORMAT_R8G8_UNORM;
                    switch(type)
                    {
                        case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32_UINT;
                        case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32_SINT;
                        case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32_FLOAT;
                    }
                }
                case 3:
                {
                    switch(type)
                    {
                        case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32B32_UINT;
                        case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32B32_SINT;
                        case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32B32_FLOAT;
                    }
                }
                case 4:
                {
                    if(semantic == "COLOR") return DXGI_FORMAT_R8G8B8A8_UNORM;
                    switch(type)
                    {
                        case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32B32A32_UINT;
                        case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32B32A32_SINT;
                        case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32B32A32_FLOAT;
                    }
                }
                default:
                return DXGI_FORMAT_UNKNOWN;
            }
        }
    }
}