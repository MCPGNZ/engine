#pragma once
#include <map>
#include "type_info.h"
#include <functional>

namespace pk
{
    class Inspector
    {
    public:
        Inspector(void);

        template <typename T, typename Function>
        void add(Function& function);

        template <typename T>
        void bind(T& data);

        void render();

    private:
        std::map<type, std::function<void(void*)>> _views;
        std::function<void(void*)> _selected = nullptr;
        void* _data = nullptr;
    };

    template <typename T, typename Function>
    void Inspector::add(Function& function)
    {
        auto t = type_info<T>::type;
        _views[t] = [&](void* data)
        {
            function(*reinterpret_cast<T*>(data));
        };
    }

    template <typename T>
    void Inspector::bind(T& data)
    {
        _selected = _views[type_info<T>::type];
        _data = &data;
    }
}