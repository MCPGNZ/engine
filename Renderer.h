#pragma once
#include "vec2.h"

#include <wrl/client.h>
#include <d3d11.h>

#include "color.h"

#include "Texture.h"
#include "Filesystem.h"
#include "GPUDesc.h"

namespace pk
{
    class RenderTarget;
    class Viewport;
    class Buffer;
    class Shader;
    class Sampler;
    class Model;
    class BlendState;

    class Renderer
    {
    public:
        #pragma region Ctors
        Renderer(void);
        #pragma endregion

        #pragma region Public Methods
        static Texture createTexture(const Image& image);
        static Texture createTexture(const std::filesystem::path& path);

        void clear(const Color& color = Color::transparent);
        void present();

        void draw(const Model& model);

        void set(RenderTarget& renderTarget);
        void set(Shader& shader);

        void set(const Viewport& viewport);
        void set(const Buffer& buffer);
        void set(const Sampler& sampler);
        void set(const Model& model);
        void set(const BlendState& blendState);
        #pragma endregion

        #pragma region Static Methods
        static ID3D11Device* _device() { return device.Get(); };
        static ID3D11DeviceContext* _context() { return context.Get(); };
        #pragma endregion

    private:
        #pragma region Private Variables
        RenderTarget* _renderTarget = nullptr;
        const Viewport* _viewport = nullptr;
        #pragma endregion

        #pragma region Static Variables
        template <typename T>
        using Com = Microsoft::WRL::ComPtr<T>;

        static Com<ID3D11Device>        device;
        static Com<ID3D11DeviceContext> context;
        #pragma endregion

        #pragma region Private Methods
        void initialize();
        void bind(const RenderTarget* renderTarget);
        void bind(const Viewport* viewport);
        void bind(const Shader* shader);
        #pragma endregion
    };
}
