#include "Renderer.h"

#include <d3dcommon.h>
#include <d3d11.h>

#include "RenderTarget.h"
#include "Viewport.h"
#include "Shader.h"

#include "logger.h"
#include "image.h"
#include "UtilityDX11.h"

#include "Model.h"
#include "Buffer.h"
#include "Sampler.h"

namespace pk
{
    #pragma region Static variables
    Renderer::Com<ID3D11Device>        Renderer::device = nullptr;
    Renderer::Com<ID3D11DeviceContext> Renderer::context = nullptr;
    #pragma endregion

    #pragma region Ctors
    Renderer::Renderer(void)
    {
        initialize();
    }
    #pragma endregion

    #pragma region Public methods
    Texture Renderer::createTexture(const std::filesystem::path& path)
    {
        return createTexture(Image{path});
    }
    Texture Renderer::createTexture(const Image& image)
    {
        Texture result{};
        result.size = image.size();

        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
        textureDesc.Width = result.size.width;
        textureDesc.Height = result.size.height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = dx::Convert(result.format);
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.MiscFlags = 0;
        textureDesc.CPUAccessFlags = 0;

        const void* data = image.data().data();

        if(data == nullptr)
        {
            auto hr = device->CreateTexture2D(&textureDesc, nullptr, result._resource.GetAddressOf());

            logger::assert_hr(hr, "Graphics", "CreateTexture", "CreateTexture2D(&textureDesc, nullptr, result.resource.GetAddressOf())");
        }
        else
        {
            D3D11_SUBRESOURCE_DATA resourceData;
            resourceData.pSysMem = data;
            resourceData.SysMemPitch = result.size.x * TextureFormatByteWidth(result.format);
            resourceData.SysMemSlicePitch = 0;

            auto hr = device->CreateTexture2D(&textureDesc, &resourceData, result._resource.GetAddressOf());
            logger::assert_hr(hr, "Graphics", "CreateTexture", "CreateTexture2D(&textureDesc, &resourceData, result.resource.GetAddressOf())");
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
        shaderViewDesc.Format = textureDesc.Format;
        shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderViewDesc.Texture2D.MipLevels = 1;
        shaderViewDesc.Texture2D.MostDetailedMip = 0;

        auto hr = device->CreateShaderResourceView(result._resource.Get(), &shaderViewDesc, result._shaderView.GetAddressOf());
        logger::assert_hr(hr, "Graphics", "CreateTexture", "CreateShaderResourceView(result.resource.Get(), &shaderViewDesc, result.shaderView.GetAddressOf())");;

        return result;
    }

    void Renderer::clear(const Color& color /*= Color::transparent*/)
    {
        logger::assert_cond(context != nullptr, "Renderer", "clear", "context == nullptr");
        logger::assert_cond(_renderTarget != nullptr, "Renderer", "clear", "target == nullptr");
        logger::assert_cond(_renderTarget->_renderTargetView.Get() != nullptr, "Renderer", "clear", "target->renderTargetView == nullptr");
        logger::assert_cond(_renderTarget->_depthStencilView.Get() != nullptr, "Renderer", "clear", "target->depthStencilView == nullptr");

        context->ClearRenderTargetView(_renderTarget->_renderTargetView.Get(), static_cast<vec4f>(color).v);
        context->ClearDepthStencilView(_renderTarget->_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void Renderer::present()
    {
        logger::assert_cond(_renderTarget != nullptr, "Renderer", "clear", "target == nullptr");
        logger::assert_cond(_renderTarget->_swapChain != nullptr, "Renderer", "clear", "target->swapChain == nullptr");

        auto hr = _renderTarget->_swapChain->Present(0, 0);
        logger::assert_hr(hr, "Renderer", "present", "target->swapChain->Present(0, 0)");
    }

    void Renderer::set(RenderTarget& renderTarget)
    {
        renderTarget._renderer = this;
        _renderTarget = &renderTarget;

        bind(_renderTarget);

        // if exists, recalculate viewport
        if(_viewport != nullptr)
        {
            bind(_viewport);
        }
    }
    void Renderer::set(Viewport& viewport)
    {
        _viewport = &viewport;
        bind(_viewport);
    }
    void Renderer::set(Buffer& buffer)
    {
        switch(buffer.type())
        {
            case BufferBind::Vertex:
            {
                UINT stride = static_cast<unsigned int>(buffer.elementSize);
                UINT offset = 0;
                context->IASetVertexBuffers(0, 1, buffer.address.GetAddressOf(), &stride, &offset);
                break;
            }

            case BufferBind::Index:
            {
                const auto stride = buffer.elementSize;
                context->IASetIndexBuffer(buffer.address.Get(), stride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
                break;
            }
        }
    }
    void Renderer::set(Shader& shader)
    {
        shader._renderer = this;
        bind(&shader);
    }
    void Renderer::set(Sampler& sampler)
    {
        context->PSSetSamplers(0, 1, &sampler._sampler);
    }
    void Renderer::set(Model& model)
    {}
    #pragma endregion

    #pragma region Private Methods
    void Renderer::initialize()
    {
        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;

        // create device
        auto hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &device,
            &featureLevel,
            &context);

        logger::assert_hr(hr, "Renderer", "initialize", "D3D11CreateDevice");
        logger::assert_cond(featureLevel >= D3D_FEATURE_LEVEL_11_0,
            "Renderer", "initialize", "D3D_FEATURE_LEVEL < D3D_FEATURE_LEVEL_11_0");
    }

    void Renderer::bind(RenderTarget* renderTarget)
    {
        // update device state
        context->RSSetState(_renderTarget->_rasterizer.Get());
        context->OMSetDepthStencilState(_renderTarget->_depthStencilState.Get(), 0);
        context->OMSetRenderTargets(1, _renderTarget->_renderTargetView.GetAddressOf(),
            _renderTarget->_depthStencilView.Get());
    }
    void Renderer::bind(Viewport* viewport)
    {
        // check, if render-target was set
        if(_renderTarget == nullptr) return;

        // transform from normalized to pixel coordinates
        D3D11_VIEWPORT vp;
        vp.TopLeftX = viewport->position.x * _renderTarget->_size.x;
        vp.TopLeftY = viewport->position.y * _renderTarget->_size.y;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.Width = viewport->scale.x * _renderTarget->_size.x;
        vp.Height = viewport->scale.y * _renderTarget->_size.y;

        // update device state
        context->RSSetViewports(1, &vp);
    }
    void Renderer::bind(Shader* shader)
    {
        // check, if vertex shader was created
        if(shader->vertexShader != nullptr)
        {
            // send vertex shader to device
            context->IASetInputLayout(shader->_layout.Get());
            context->VSSetShader(shader->vertexShader.Get(), nullptr, 0);

            // send constant buffers associated with vertex shader
            for(const auto& bufferData : shader->constantBuffers)
            {
                context->VSSetConstantBuffers(bufferData.index, 1, bufferData.buffer.address.GetAddressOf());
            }
        }

        // check, if pixel shader was created
        if(shader->pixelShader != nullptr)
        {
            // send pixel shader to device
            context->PSSetShader(shader->pixelShader.Get(), nullptr, 0);

            // send constant buffers associated with pixel shader
            for(const auto& bufferData : shader->constantBuffers)
            {
                context->PSSetConstantBuffers(bufferData.index, 1, bufferData.buffer.address.GetAddressOf());
            }
        }
    }
    #pragma endregion
}