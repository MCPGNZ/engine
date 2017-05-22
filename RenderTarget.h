#pragma once
#include "vec2.h"

#include <wrl/client.h>
#include <d3d11.h>

#include "GPUDesc.h"
#include "window_handle.h"

namespace pk
{
    class Renderer;

    class RenderTarget
    {
    public:
        #pragma region Ctors
        RenderTarget(const window_handle& handle, const vec2i& size);
        #pragma endregion

        #pragma region Public Methods
        void create(const window_handle& handle, const vec2i& size);
        void resize(const vec2i& size);
        #pragma endregion

        #pragma region Operators
        bool operator==(const RenderTarget& other) const;
        bool operator!=(const RenderTarget& other) const;
        #pragma endregion

    private:
        #pragma region Variables
        template <typename T>
        using Com = Microsoft::WRL::ComPtr<T>;

        Com<IDXGISwapChain>         _swapChain = nullptr;
        Com<ID3D11RenderTargetView> _renderTargetView = nullptr;

        Com<ID3D11Texture2D>         _depthStencil = nullptr;
        Com<ID3D11DepthStencilView>  _depthStencilView = nullptr;
        Com<ID3D11DepthStencilState> _depthStencilState = nullptr;

        Com<ID3D11RasterizerState>  _rasterizer = nullptr;

        TextureFormat _format = TextureFormat::Rgba8Unorm;

        window_handle _handle;
        vec2i _size;

        Renderer* _renderer;
        #pragma endregion

        #pragma region Private Methods
        void create_swapchain(const window_handle& handle, const vec2i& size);
        void create_render_target_view();
        void create_depth_stencil_view();
        void create_depth_stencil_state();
        void create_rasterizer();
        #pragma endregion

        friend class Renderer;
        friend class Viewport;
    };
}