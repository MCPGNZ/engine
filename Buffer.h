#pragma once
#include <stdlib.h>

#include <d3d11.h>
#include <wrl\client.h>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include "GPUDesc.h"

namespace pk
{
    #pragma region Usings
    using Microsoft::WRL::ComPtr;
    #pragma endregion

    class Buffer
    {
    public:
        #pragma region Ctors
        Buffer(void) = default;
        #pragma endregion

        #pragma region Operators
        bool operator== (const Buffer& other) const;
        bool operator!= (const Buffer& other) const;
        #pragma endregion

        #pragma region Public methods
        static Buffer constant(const void* data, std::int32_t bytes);
        static Buffer vertex(const void* data, std::int32_t bytes, std::int32_t stride);
        static Buffer index(const void* data, std::int32_t bytes, std::int32_t stride = sizeof(std::uint32_t));

        void create(const void* data, std::int32_t bytes, std::int32_t stride, BufferBind bind, BufferUsage usage);

        BufferBind type() const;
        bool empty() const;
        std::int32_t count() const
        {
            if(elementSize == 0)
            {
                return 0;
            }

            return bytes / elementSize;
        }

        void send(void* data);
        int8_t* lock();
        void unlock();
        #pragma endregion

    public:
        #pragma region Private variables
        ComPtr<ID3D11Buffer> address = nullptr;

        std::int32_t bytes = 0;
        std::int32_t elementSize = 0;

        BufferBind _bind;
        #pragma endregion
    };
}