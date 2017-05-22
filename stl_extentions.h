#pragma once
#include <algorithm>

namespace std
{
    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    bool contains(const Container<T, Args...>& container, const T& element)
    {
        return container.find(element) != container.cend();
    };

    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    T* find(Container<T, Args...>& containter, const T& element)
    {
        auto& it = std::find(std::begin(containter), std::end(containter), element);
        return (it == std::end(containter)) ? nullptr : &*it;
    };

    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    void operator+=(Container<T, Args...>& first, const Container<T, Args...>& second)
    {
        first.reserve(first.size() + second.size());
        for(auto& entry : second)
        {
            first.push_back(entry);
        }
    }
}