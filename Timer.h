#pragma once
#include <chrono>
#include <functional>

namespace pk
{
    class Timer
    {
    public:
        Timer(void);

        template <typename T = std::chrono::milliseconds, typename Function>
        static auto time(const Function& function);

        template <typename T>
        static auto current();

        template <typename T = std::chrono::seconds>
        auto delta();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _timePoint;
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

    template <typename T>
    auto Timer::delta()
    {
        const auto previous = _timePoint;
        _timePoint = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> duration = _timePoint - previous;
        return duration.count();
    }
}