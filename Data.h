#pragma once
#include <vector>
#include "stl_extentions.h"

namespace pk
{
    template <typename T>
    class Data
    {
    public:
        static int create();
        static std::vector<T> _components;
    };

    template <typename T>
    int Data<T>::create()
    {
        _components.emplace_back();
        return std::lastIndex(_components);
    }

    template <typename T>
    std::vector<T> Data<T>::_components;
}