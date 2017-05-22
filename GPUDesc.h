#pragma once
#include <string>

namespace pk
{
    enum struct Blend
    {
        Disable,
        Alpha,
        Additive,
        Min,
        Max
    };
    enum struct Depth
    {
        WriteEnable,
        WriteDisable
    };

    enum struct ShaderLayout
    {
        Position = 0x001,
        Normal = 0x002,
        Binormal = 0x004,
        Tangent = 0x008,
        Texcoord = 0x010,
        Color = 0x020,
    };

    #pragma region Public types
    enum struct TextureFormat
    {
        Unknown = 0,

        //32 bits per channel
        Rgba32Typeless,
        Rgba32Float,
        Rgba32Uint,
        Rgba32Int,

        Rgb32Typeless,
        Rgb32Float,
        Rgb32Uint,
        Rgb32Int,

        Rg32Typeless,
        Rg32Float,
        Rg32Uint,
        Rg32Int,

        R32Typeless,
        R32Float,
        R32Uint,
        R32Int,

        D32Float,

        //16 bits per channel
        Rgba16Typeless,
        Rgba16Float,
        Rgba16Uint,
        Rgba16Int,
        Rgba16Unorm,

        Rg16Typeless,
        Rg16Float,
        Rg16Uint,
        Rg16Int,
        Rg16Unorm,

        R16Typeless,
        R16Float,
        R16Uint,
        R16Int,
        R16Unorm,

        //8 bits per channel
        Rgba8Typeless,
        Rgba8Uint,
        Rgba8Int,
        Rgba8Unorm,

        R8Typeless,
        R8Uint,
        R8Int,
        R8Unorm
    };
    enum struct Topology
    {
        Undefined,
        LineList,
        LineStrip,
        PointList,
        TriangleList,
        TriangleStrip
    };
    enum struct BufferCpuAccess
    {
        None,
        Read,
        Write,
        ReadWrite
    };
    enum struct BufferUsage
    {
        Static,
        Dynamic,
        Staging
    };
    enum struct BufferBind
    {
        Vertex,
        Index,
        ConstantBuffer
    };
    enum struct BufferLayout
    {
        Interleaving,
        Linear
    };
    enum struct ShaderType
    {
        Vertex,
        Geometry,
        Pixel
    };

    enum struct IADataType
    {
        Vertex,
        Instance
    };

    struct BufferDesc
    {
        BufferUsage usage = BufferUsage::Dynamic;
        BufferBind bind = BufferBind::Vertex;
        BufferCpuAccess access = BufferCpuAccess::Read;

        void* data = nullptr;
        int byteWidth = 0;
    };
    struct TextureDesc
    {
        std::string path;
        int width = 0;
        int height = 0;
    };
    #pragma endregion

    #pragma region Public functions
    static inline std::string StringToFormat(const TextureFormat format)
    {
        //32 bits per channel
        if(format == TextureFormat::Rgba32Typeless) return "RGBA32_TYPELESS";
        if(format == TextureFormat::Rgba32Float)    return "RGBA32_FLOAT";
        if(format == TextureFormat::Rgba32Uint)     return "RGBA32_UINT";
        if(format == TextureFormat::Rgba32Int)      return "RGBA32_INT";

        if(format == TextureFormat::Rgb32Typeless) return "RGB32_TYPELESS";
        if(format == TextureFormat::Rgb32Float)    return "RGB32_FLOAT";
        if(format == TextureFormat::Rgb32Uint)     return "RGB32_UINT";
        if(format == TextureFormat::Rgb32Int)      return "RGB32_INT";

        if(format == TextureFormat::Rg32Typeless) return "RG32_TYPELESS";
        if(format == TextureFormat::Rg32Float)    return "RG32_FLOAT";
        if(format == TextureFormat::Rg32Uint)     return "RG32_UINT";
        if(format == TextureFormat::Rg32Int)      return "RG32_INT";

        if(format == TextureFormat::R32Typeless) return "R32_TYPELESS";
        if(format == TextureFormat::R32Float)    return "R32_FLOAT";
        if(format == TextureFormat::R32Uint)     return "R32_UINT";
        if(format == TextureFormat::R32Int)      return "R32_INT";

        if(format == TextureFormat::D32Float) return "D32_FLOAT";

        //16 bits per channel
        if(format == TextureFormat::Rgba16Typeless) return "RGBA16_TYPELESS";
        if(format == TextureFormat::Rgba16Float)    return "RGBA16_FLOAT";
        if(format == TextureFormat::Rgba16Uint)     return "RGBA16_UINT";
        if(format == TextureFormat::Rgba16Int)      return "RGBA16_INT";
        if(format == TextureFormat::Rgba16Unorm)    return "RGBA16_UNORM";

        if(format == TextureFormat::Rg16Typeless) return "RG16_TYPELESS";
        if(format == TextureFormat::Rg16Float)    return "RG16_FLOAT";
        if(format == TextureFormat::Rg16Uint)     return "RG16_UINT";
        if(format == TextureFormat::Rg16Int)      return "RG16_INT";
        if(format == TextureFormat::Rg16Unorm)    return "RG16_UNORM";

        if(format == TextureFormat::R16Typeless) return "R16_TYPELESS";
        if(format == TextureFormat::R16Float)    return "R16_FLOAT";
        if(format == TextureFormat::R16Uint)     return "R16_UINT";
        if(format == TextureFormat::R16Int)      return "R16_INT";
        if(format == TextureFormat::R16Unorm)    return "R16_UNORM";

        //8 bits per channel
        if(format == TextureFormat::Rgba8Typeless) return "RGBA8_TYPELESS";
        if(format == TextureFormat::Rgba8Uint)     return "RGBA8_UINT";
        if(format == TextureFormat::Rgba8Int)      return "RGBA8_INT";
        if(format == TextureFormat::Rgba8Unorm)    return "RGBA8_UNORM";

        if(format == TextureFormat::R8Typeless) return "R8_TYPELESS";
        if(format == TextureFormat::R8Uint)     return "R8_UINT";
        if(format == TextureFormat::R8Int)      return "R8_INT";
        if(format == TextureFormat::R8Unorm)    return "R8_UNORM";

        return "UNKNOWN";
    }
    static inline std::string FormatToValueName(const TextureFormat format)
    {
        if(format == TextureFormat::Rgba32Float) return "float4";
        if(format == TextureFormat::Rgba32Uint)  return "uint4";
        if(format == TextureFormat::Rgba32Int)   return "int4";

        if(format == TextureFormat::Rgb32Float)  return "float3";
        if(format == TextureFormat::Rgb32Uint)   return "uint3";
        if(format == TextureFormat::Rgb32Int)    return "int3";

        if(format == TextureFormat::Rg32Float)   return "float2";
        if(format == TextureFormat::Rg32Uint)    return "uint2";
        if(format == TextureFormat::Rg32Int)     return "int2";

        if(format == TextureFormat::R32Float)    return "float";
        if(format == TextureFormat::R32Uint)     return "uint";
        if(format == TextureFormat::R32Int)      return "int";

        if(format == TextureFormat::D32Float)    return "float";

        return "UNKNOWN";
    }
    static inline TextureFormat   FormatFromString(const std::string& format)
    {
        //32 bits per channel
        if(format == "RGBA32_TYPELESS")   return TextureFormat::Rgba32Typeless;
        if(format == "RGBA32_FLOAT")      return TextureFormat::Rgba32Float;
        if(format == "RGBA32_UINT")       return TextureFormat::Rgba32Uint;
        if(format == "RGBA32_INT")        return TextureFormat::Rgba32Int;

        if(format == "RGB32_TYPELESS")    return TextureFormat::Rgb32Typeless;
        if(format == "RGB32_FLOAT")       return TextureFormat::Rgb32Float;
        if(format == "RGB32_UINT")        return TextureFormat::Rgb32Uint;
        if(format == "RGB32_INT")         return TextureFormat::Rgb32Int;

        if(format == "RG32_TYPELESS")     return TextureFormat::Rg32Typeless;
        if(format == "RG32_FLOAT")        return TextureFormat::Rg32Float;
        if(format == "RG32_UINT")         return TextureFormat::Rg32Uint;
        if(format == "RG32_INT")          return TextureFormat::Rg32Int;

        if(format == "R32_TYPELESS")      return TextureFormat::R32Typeless;
        if(format == "R32_FLOAT")         return TextureFormat::R32Float;
        if(format == "R32_UINT")          return TextureFormat::R32Uint;
        if(format == "R32_INT")           return TextureFormat::R32Int;

        if(format == "D32_FLOAT")         return TextureFormat::D32Float;

        //16 bits per channel
        if(format == "RGBA16_TYPELESS")   return TextureFormat::Rgba16Typeless;
        if(format == "RGBA16_FLOAT")      return TextureFormat::Rgba16Float;
        if(format == "RGBA16_UINT")       return TextureFormat::Rgba16Uint;
        if(format == "RGBA16_INT")        return TextureFormat::Rgba16Int;
        if(format == "RGBA16_UNORM")      return TextureFormat::Rgba16Unorm;

        if(format == "RG16_TYPELESS")     return TextureFormat::Rg16Typeless;
        if(format == "RG16_FLOAT")        return TextureFormat::Rg16Float;
        if(format == "RG16_UINT")         return TextureFormat::Rg16Uint;
        if(format == "RG16_INT")          return TextureFormat::Rg16Int;
        if(format == "RG16_UNORM")        return TextureFormat::Rg16Unorm;

        if(format == "R16_TYPELESS")      return TextureFormat::R16Typeless;
        if(format == "R16_FLOAT")         return TextureFormat::R16Float;
        if(format == "R16_UINT")          return TextureFormat::R16Uint;
        if(format == "R16_INT")           return TextureFormat::R16Int;
        if(format == "R16_UNORM")         return TextureFormat::R16Unorm;

        //8 bits per channel
        if(format == "RGBA8_TYPELESS")    return TextureFormat::Rgba8Typeless;
        if(format == "RGBA8_UINT")        return TextureFormat::Rgba8Uint;
        if(format == "RGBA8_INT")         return TextureFormat::Rgba8Int;
        if(format == "RGBA8_UNORM")       return TextureFormat::Rgba8Unorm;

        if(format == "R8_TYPELESS")       return TextureFormat::R8Typeless;
        if(format == "R8_UINT")           return TextureFormat::R8Uint;
        if(format == "R8_INT")            return TextureFormat::R8Int;
        if(format == "R8_UNORM")          return TextureFormat::R8Unorm;

        return TextureFormat::Unknown;
    }
    static inline int TextureFormatByteWidth(const TextureFormat format)
    {
        switch(format)
        {
            case TextureFormat::Rgba32Typeless: return 16;
            case TextureFormat::Rgba32Float:    return 16;
            case TextureFormat::Rgba32Uint:     return 16;
            case TextureFormat::Rgba32Int:      return 16;
            case TextureFormat::Rgb32Typeless:  return 12;
            case TextureFormat::Rgb32Float:     return 12;
            case TextureFormat::Rgb32Uint:      return 12;
            case TextureFormat::Rgb32Int:       return 12;
            case TextureFormat::Rg32Typeless:   return 8;
            case TextureFormat::Rg32Float:      return 8;
            case TextureFormat::Rg32Uint:       return 8;
            case TextureFormat::Rg32Int:        return 8;
            case TextureFormat::R32Typeless:    return 4;
            case TextureFormat::R32Float:       return 4;
            case TextureFormat::R32Uint:        return 4;
            case TextureFormat::R32Int:         return 4;
            case TextureFormat::D32Float:       return 4;
            case TextureFormat::Rgba16Typeless: return 8;
            case TextureFormat::Rgba16Float:    return 8;
            case TextureFormat::Rgba16Uint:     return 8;
            case TextureFormat::Rgba16Int:      return 8;
            case TextureFormat::Rgba16Unorm:    return 8;
            case TextureFormat::Rg16Typeless:   return 4;
            case TextureFormat::Rg16Float:      return 4;
            case TextureFormat::Rg16Uint:       return 4;
            case TextureFormat::Rg16Int:        return 4;
            case TextureFormat::Rg16Unorm:      return 4;
            case TextureFormat::R16Typeless:    return 2;
            case TextureFormat::R16Float:       return 2;
            case TextureFormat::R16Uint:        return 2;
            case TextureFormat::R16Int:         return 2;
            case TextureFormat::R16Unorm:       return 2;
            case TextureFormat::Rgba8Typeless:  return 4;
            case TextureFormat::Rgba8Uint:      return 4;
            case TextureFormat::Rgba8Int:       return 4;
            case TextureFormat::Rgba8Unorm:     return 4;
            case TextureFormat::R8Typeless:     return 1;
            case TextureFormat::R8Uint:         return 1;
            case TextureFormat::R8Int:          return 1;
            case TextureFormat::R8Unorm:        return 1;
        }

        return -1;
    }
    static inline Topology TopologyFromString(const std::string& topology)
    {
        if(topology == "LINELIST")      return Topology::LineList;
        if(topology == "LINESTRIP")     return Topology::LineStrip;
        if(topology == "POINTLIST")     return Topology::PointList;
        if(topology == "TRIANGLELIST")  return Topology::TriangleList;
        if(topology == "TRAINGLESTRIP") return Topology::TriangleStrip;

        return Topology::Undefined;
    }
    #pragma endregion
}