#pragma once
#include <chrono>
#include <functional>

namespace pk
{
    class Timer
    {
    public:
        template <typename T = std::chrono::milliseconds, typename Function>
        static auto time(const Function& function);

        template <typename T>
        static auto current();
    };

    template <typename T, typename Function>
    auto Timer::time(const Function& function)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        function();
        const auto end = std::chrono::high_resolution_clock::now();

        return std::chrono::duration_cast<T>(end - start);
    }

    template <typename T>
    auto Timer::current()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<T>(now);
    }
}