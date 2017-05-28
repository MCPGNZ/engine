#include "RenderTarget.h"

#include <d3d11.h>

#include "Renderer.h"
#include "UtilityDX11.h"
#include "logger.h"

#include "window.h"

namespace pk
{
    #pragma region Ctors
    RenderTarget::RenderTarget(const window_handle& handle, const vec2i& size) :
        _handle{handle}, _size{size}
    {
        create(_handle, _size);
    }
    #pragma endregion

    #pragma region Public methods
    void RenderTarget::create(const window_handle& handle, const vec2i& size)
    {
        _handle = handle;
        _size = size;

        create_swapchain(_handle, _size);
        create_render_target_view();
        create_depth_stencil_view();
        create_rasterizer();
    }
    void RenderTarget::resize(const vec2i& size)
    {
        // check parameters
        logger::assert_cond(size.x > 0 && size.y > 0, "RenderTarget", "resize", "passed size <= 0");

        // check state
        logger::assert_cond(_handle != nullptr, "RenderTarget", "resize", "_handle is nullptr");
        logger::assert_cond(_renderer != nullptr, "RenderTarget", "resize", "_renderer is nullptr");

        create(_handle, size);
        _renderer->set(*this);
    }
    #pragma endregion

    #pragma region Operators
    bool RenderTarget::operator==(const RenderTarget& other) const
    {
        if(_size != other._size)
            return false;

        if(_format != other._format)
            return false;

        if(_swapChain != nullptr && other._swapChain != nullptr)
        {
            if(_swapChain.GetAddressOf() != other._swapChain.GetAddressOf())
            {
                return false;
            }
        }

        if(_renderTargetView != nullptr && other._renderTargetView != nullptr)
        {
            if(_renderTargetView.GetAddressOf() != other._renderTargetView.GetAddressOf())
            {
                return false;
            }
        }

        return true;
    }
    bool RenderTarget::operator!=(const RenderTarget& other) const
    {
        return !operator==(other);
    }
    #pragma endregion

    #pragma region Private methods
    void RenderTarget::create_swapchain(const window_handle& handle, const vec2i& size)
    {
        // release swap-chain in case it was created before
        _swapChain = nullptr;

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChainDesc.BufferDesc.Width = size.width;
        swapChainDesc.BufferDesc.Height = size.height;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Format = dx::Convert(_format);
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.OutputWindow = handle;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        HRESULT hr;
        Com<IDXGIDevice>  dxgiDevice = nullptr;
        Com<IDXGIAdapter> dxgiAdapter = nullptr;
        Com<IDXGIFactory> dxgiFactory = nullptr;

        logger::assert_cond(Renderer::_device() != nullptr, "RenderTexture", "create_swapchain",
            "Renderer not initialized, call to _device() will fail,"
            "Create Renderer before calling RenderTexture methods");

        const auto& device = Renderer::_device();

        hr = device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf()));
        logger::assert_hr(hr, "RenderTarget", "create_swapchain", "QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf()))");

        hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(dxgiAdapter.GetAddressOf()));
        logger::assert_hr(hr, "RenderTarget", "create_swapchain", "GetParent(__uuidof(IDXGIAdapter), (void**) reinterpret_cast<void**>(dxgiAdapter.GetAddressOf()))");

        hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
        logger::assert_hr(hr, "RenderTarget", "create_swapchain", "GetParent(__uuidof(IDXGIFactory), (void**) reinterpret_cast<void**>(dxgiFactory.GetAddressOf()))");

        hr = dxgiFactory->CreateSwapChain(device, &swapChainDesc, _swapChain.GetAddressOf());
        logger::assert_hr(hr, "RenderTarget", "create_swapchain", "CreateSwapChain(device.Get(), &swapChainDesc, &result.swapChain)");
    }
    void RenderTarget::create_render_target_view()
    {
        // release render-target-view in case it was created before
        _renderTargetView = nullptr;

        const auto& device = Renderer::_device();

        Com<ID3D11Texture2D> backBuffer;

        auto hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        logger::assert_hr(hr, "RenderTarget", "ccreate_render_target_viewreate", "GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))");

        hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
        logger::assert_hr(hr, "RenderTarget", "create_render_target_view", "CreateRenderTargetView(backBuffer.Get(), nullptr, result.renderTargetView.GetAddressOf())");
    }
    void RenderTarget::create_depth_stencil_view()
    {
        // release depth-stencil and depth-stencil-view in case there were created before
        _depthStencil = nullptr;
        _depthStencilView = nullptr;

        const auto& device = Renderer::_device();

        D3D11_TEXTURE2D_DESC depthBufferDesc = {0};
        depthBufferDesc.Width = _size.x;
        depthBufferDesc.Height = _size.y;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;

        auto hr = device->CreateTexture2D(&depthBufferDesc, NULL, _depthStencil.GetAddressOf());
        logger::assert_hr(hr, "RenderTarget", "create_depth_stencil_view", "CreateTexture2D (depthStencilBuffer)");

        D3D11_DEPTH_STENCIL_VIEW_DESC  depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

        depthStencilViewDesc.Format = depthBufferDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        hr = device->CreateDepthStencilView(_depthStencil.Get(), &depthStencilViewDesc, _depthStencilView.GetAddressOf());
        logger::assert_hr(hr, "RenderTarget", "create_depth_stencil_view", "CreateDepthStencilView");
    }

    void RenderTarget::create_rasterizer()
    {
        // release rasterizer in case it was created before
        _rasterizer = nullptr;

        const auto& device = Renderer::_device();

        D3D11_RASTERIZER_DESC rasterizerStateDesc;
        ZeroMemory(&rasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));
        rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
        rasterizerStateDesc.ScissorEnable = false;
        rasterizerStateDesc.DepthClipEnable = true;

        auto hr = device->CreateRasterizerState(&rasterizerStateDesc, _rasterizer.GetAddressOf());
        logger::assert_hr(hr, "RenderTarget", "create_rasterizer", "CreateRasterizerState(&rasterizerStateDesc, rasterizer.GetAddressOf()");
    }
    #pragma endregion
}