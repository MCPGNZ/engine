#pragma once
namespace pk
{
    template <typename T>
    class Singleton
    {
    public:
        static T& instance()
        {
            static T object{};
            return object;
        }
    };
}
