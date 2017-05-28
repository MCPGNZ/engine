#pragma once
#include <algorithm>
#include <numeric>

namespace std
{
    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    T* find(Container<T, Args...>& containter, const T& element)
    {
        auto& it = std::find(std::begin(containter), std::end(containter), element);
        return (it == std::end(containter)) ? nullptr : &*it;
    };

    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    int findIndex(Container<T, Args...>& containter, const T& element)
    {
        auto& it = std::find(std::begin(containter), std::end(containter), element);
        return (it == std::end(containter)) ? std::numeric_limits<int>::min() : std::distance(std::begin(containter), it);
    };

    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    T* last(Container<T, Args...>& containter)
    {
        const int size = static_cast<int>(containter.size());
        if(size == 0)
        {
            return nullptr;
        }

        return &containter[containter.size() - 1];
    };

    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    int lastIndex(Container<T, Args...>& containter)
    {
        const int size = static_cast<int>(containter.size());
        if(size == 0)
        {
            return std::numeric_limits<int>::min();
        }

        return size - 1;
    };

    template <template <typename T, typename... Args> typename Container, typename T, typename... Args>
    bool contains(const Container<T, Args...>& container, const T& element)
    {
        return std::find(container.cbegin(), container.cend(), element) != container.cend();
    };

    bool isValid(const int value);

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