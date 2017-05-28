#pragma once
#include "type_info.h"
namespace pk
{
    struct DragInfo
    {
        type _type;
        void* _data;

        template <typename T> bool isValid();
        template <typename T> T* get();

        static DragInfo interpret(void* data);
    };

    template<typename T>
    bool DragInfo::isValid()
    {
        return type_info<T>::type == _type;
    }

    template<typename T>
    T* DragInfo::get()
    {
        return isValid<T>() ? reinterpret_cast<T*>(_data) : nullptr;
    }
}
