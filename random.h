#pragma once

#include <random>
#include "traits.h"

namespace pk
{
    class random
    {
    private:
        static std::random_device device;
        static std::mt19937 generator;

    public:
        template <typename T>
        static float range(T min, T max, IS_FLOATING(T))
        {
            std::uniform_real_distribution<T> distribution{min, max};
            return distribution(generator);
        }

        template <typename T>
        static T range(T min, T max, IS_INTEGRAL(T))
        {
            std::uniform_int_distribution<T> distribution{min, max};
            return distribution(generator);
        }

        template< template<typename T, typename A = std::allocator<T>> class Collection, typename T>
        static T value(const Collection<T>& collection)
        {
            return collection[range(0, static_cast<int>(collection.size() - 1))];
        }
    };
}
