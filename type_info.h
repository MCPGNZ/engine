#pragma once
#include <string>

//////////////////////////////////////////////////////////////////////////
// [status]: needs revision
//////////////////////////////////////////////////////////////////////////

namespace pk
{
    class type
    {
    public:
        #pragma region Ctors
        type(void) = default;
        explicit type(const char* data) : _name{data} {};
        #pragma endregion

        #pragma region Operators
        bool operator==(const type& other) const
        {
            return _name == other._name;
        }
        bool operator!=(const type& other) const
        {
            return _name != other._name;
        }
        bool operator<(const type& other) const
        {
            return _name < other._name;
        }
        #pragma endregion

        #pragma region Public Methods
        const char* name() const
        {
            return _name;
        }
        #pragma endregion

    private:
        const char* _name;
    };

    template <typename T> struct type_info
    {
        static type type;
        static const char* name()
        {
            return __FUNCSIG__;
        };
    };

    template<class T> type type_info< T >::type(type_info< T >::name());

    template<class T> struct type_info< T & > : type_info< T > {};
    template<class T> struct type_info< T const > : type_info< T > {};
    template<class T> struct type_info< T volatile > : type_info< T > {};
    template<class T> struct type_info< T const volatile > : type_info< T > {};
}