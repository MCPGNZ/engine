#include "Buffer.h"

#include "Renderer.h"
#include "UtilityDX11.h"
#include "logger.h"

namespace pk
{
    #pragma region Operators
    bool Buffer::operator==(const Buffer& other) const
    {
        return (address == other.address) &&
            (bytes == other.bytes) &&
            (elementSize == other.elementSize) &&
            (_bind == other._bind);
    }

    bool Buffer::operator!=(const Buffer& other) const
    {
        return !operator==(other);
    }
    #pragma endregion

    #pragma region Public methods

    Buffer Buffer::constant(const void* data, std::int32_t size)
    {
        Buffer result;
        result.create(data, size, size, BufferBind::ConstantBuffer, BufferUsage::Dynamic);

        return result;
    }

    pk::Buffer Buffer::vertex(const void* data, std::int32_t size, std::int32_t stride)
    {
        Buffer result;
        result.create(data, size, stride, BufferBind::Vertex, BufferUsage::Static);

        return result;
    }

    pk::Buffer Buffer::index(const void* data, std::int32_t size, std::int32_t stride /*= sizeof(std::uint32_t)*/)
    {
        Buffer result;
        result.create(data, size, stride, BufferBind::Index, BufferUsage::Static);

        return result;
    }

    void Buffer::create(const void* data, std::int32_t _bytes, std::int32_t _elementSize, BufferBind bind, BufferUsage usage)
    {
        address.Reset();

        bytes = _bytes;
        elementSize = _elementSize;

        _bind = bind;

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.Usage = dx::Convert(usage);
        bufferDesc.BindFlags = dx::Convert(bind);
        bufferDesc.CPUAccessFlags = (usage == BufferUsage::Dynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
        bufferDesc.ByteWidth = ((bytes + 15) / 16) * 16;
        bufferDesc.MiscFlags = 0;

        if(data == nullptr)
        {
            auto hr = Renderer::_device()->CreateBuffer(&bufferDesc, nullptr, address.GetAddressOf());
            logger::assert_hr(hr, "Buffer", "Create", "CreateBuffer(&bufferDesc, nullptr, address.GetAddressOf());");
            return;
        }

        D3D11_SUBRESOURCE_DATA subresourceData;
        subresourceData.pSysMem = data;
        subresourceData.SysMemPitch = 0;
        subresourceData.SysMemSlicePitch = 0;

        auto hr = Renderer::_device()->CreateBuffer(&bufferDesc, &subresourceData, address.GetAddressOf());
        logger::assert_hr(hr, "Buffer", "Create", "CreateBuffer(&bufferDesc, &subresourceData, address.GetAddressOf());");
    }

    BufferBind Buffer::type() const
    {
        return _bind;
    }

    bool Buffer::empty() const
    {
        return address == nullptr;
    }

    void Buffer::send(void* _data)
    {
        logger::assert_cond(address.Get() != nullptr, "Buffer", "data", "address == nullptr");

        D3D11_MAPPED_SUBRESOURCE resource;
        auto hr = Renderer::_context()->Map(address.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
        {
            logger::assert_hr(hr, "Buffer", "data", "Map");
            memcpy(resource.pData, _data, bytes);
        }
        Renderer::_context()->Unmap(address.Get(), 0);
    }

    int8_t* Buffer::lock()
    {
        logger::assert_cond(address.Get() != nullptr, "Buffer", "data", "address == nullptr");

        D3D11_MAPPED_SUBRESOURCE resource;
        auto hr = Renderer::_context()->Map(address.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
        logger::assert_hr(hr, "Buffer", "data", "Map");

        return reinterpret_cast<int8_t*>(resource.pData);
    }

    void Buffer::unlock()
    {
        Renderer::_context()->Unmap(address.Get(), 0);
    }
    #pragma endregion
}