#pragma once
#include <new>
#include "config.h"

namespace pk
{
    struct memory_block
    {
    public:
        #pragma region Public variables
        memory_address pointer = nullptr;
        size_type size = 0;
        #pragma endregion

        #pragma region Ctors
        memory_block(void) = default;
        memory_block(void* p, const size_type s) : pointer{reinterpret_cast<memory_address>(p)}, size{s} {};

        memory_block(const memory_block& other) = default;
        memory_block& operator=(const memory_block& other) = default;

        memory_block(memory_block&&) = default;
        memory_block& operator=(memory_block&&) = default;
        #pragma endregion

        #pragma region Public methods
        bool is_valid() const;
        bool contains(const memory_address memory) const;

        template <typename T>
        T* get_as() const;

        template <typename T, typename... Args>
        T* construct(Args&&... args) const;

        template <typename T, typename... Args>
        T* construct_array_elem(const size_type index, Args&&... args) const;

        template <typename T>
        T& get_array_elem(const size_type index);

        template <typename T>
        const T& get_array_elem(const size_type index) const;
        #pragma endregion
    };

    #pragma region Public templates
    template <typename T>
    T* memory_block::get_as() const
    {
        return reinterpret_cast<T*>(pointer);
    }

    template <typename T, typename... Args>
    T* memory_block::construct(Args&&... args) const
    {
        new (pointer) T{std::forward<Args>(args)...};
        return reinterpret_cast<T*>(pointer);
    }

    template <typename T, typename... Args>
    T* memory_block::construct_array_elem(const size_type index, Args&&... args) const
    {
        const auto address = pointer + sizeof(T) * index;

        new (address) T{std::forward<Args>(args)...};
        return reinterpret_cast<T*>(address);
    }

    template <typename T>
    T& memory_block::get_array_elem(const size_type index)
    {
        return *reinterpret_cast<T*>(pointer + sizeof(T) * index);
    }

    template <typename T>
    const T& memory_block::get_array_elem(const size_type index) const
    {
        return *reinterpret_cast<const T*>(pointer + sizeof(T) * index);
    }
    #pragma endregion
}