#pragma once
#include "vec2.h"

#include <wrl\client.h>
#include <d3d11.h>

#include "GPUDesc.h"
#include "Filesystem.h"

namespace pk
{
    class Image;
    class Texture
    {
    public:
        #pragma region Ctors
        Texture(void) = default;
        Texture(const std::filesystem::path& path);
        Texture(const Image& image);

        bool operator==(const Texture& other) const;
        #pragma endregion

        #pragma region Public methods
        void create(const std::filesystem::path& path);
        void create(const Image& image);

        void* id() const;
        #pragma endregion

        vec2i size;
        TextureFormat format = TextureFormat::Rgba8Unorm;

    private:
        template <typename T>
        using Com = Microsoft::WRL::ComPtr<T>;

        Com<ID3D11Texture2D> _resource = nullptr;
        Com<ID3D11ShaderResourceView> _shaderView = nullptr;

        ID3D11ShaderResourceView* const* _shaderResourceViewAddres() const { return _shaderView.GetAddressOf(); }

        friend class Renderer;
        friend class Shader;
    };
}
