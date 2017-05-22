// last updated: 31-03-17 [in NRDF]

#pragma once
#include <type_traits>
#include <tuple>

namespace pk
{
    //////////////////////////////////////////////////////////////////////////
    // [template typename]: Used for checking input types
    //////////////////////////////////////////////////////////////////////////
    #define IS_TRUE_TYPE(T)  typename std::enable_if<T::value>::type
    #define IS_FALSE_TYPE(T) typename std::enable_if<!T::value>::type

    #define IS_SAME_TYPE(T, U) typename std::enable_if<std::is_same<T, U>::value>::type
    #define IS_DIFF_TYPE(T, U) typename std::enable_if<!std::is_same<T, U>::value>::type

    #define IS_FLOATING_TYPE(T, U) typename std::enable_if<std::is_same<T, U>::value>::type
    #define IS_INTEGRAL_TYPE(T, U) typename std::enable_if<!std::is_same<T, U>::value>::type
    #define IS_ARITHMETIC_TYPE(T, U) typename std::enable_if<!std::is_same<T, U>::value>::type

    //////////////////////////////////////////////////////////////////////////
    // [template typename]: Used for checking input value
    //////////////////////////////////////////////////////////////////////////
    #define IS_TRUE_VALUE(T)  typename std::enable_if<T>::type
    #define IS_FALSE_VALUE(T) typename std::enable_if<!T>::type

    //////////////////////////////////////////////////////////////////////////
    // [method parameter declarations]: Used when we want to dispatch methods for T
    //////////////////////////////////////////////////////////////////////////
    #define IS_TRUE(T)       typename std::enable_if<T::value>::type* _sfinae_ = 0
    #define IS_FALSE(T)      typename std::enable_if<!T::value>::type* _sfinae_ = 0

    #define IS_SAME(T, U)    typename std::enable_if<<std::is_same<T, U>::value>::type* _sfinae_ = 0
    #define IS_DIFF(T, U)    typename std::enable_if<<!std::is_same<T, U>::value>::type* _sfinae_ = 0

    #define IS_FLOATING(T)   typename std::enable_if<std::is_floating_point<T>::value>::type* _sfinae_ = 0
    #define IS_INTEGRAL(T)   typename std::enable_if<std::is_integral<T>::value>::type* _sfinae_ = 0
    #define IS_ARITHMETIC(T) typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type* _sfinae_ = 0

    //////////////////////////////////////////////////////////////////////////
    // [method parameter definitions]: Used when we want to dispatch methods for T
    //////////////////////////////////////////////////////////////////////////
    #define IS_TRUE_DEF(T)       typename std::enable_if<T::value>::type*
    #define IS_FALSE_DEF(T)      typename std::enable_if<!T::value>::type*

    #define IS_SAME_DEF(T, U)    typename std::enable_if<<std::is_same<T, U>::value>::type*
    #define IS_DIFF_DEF(T, U)    typename std::enable_if<<!std::is_same<T, U>::value>::type*

    #define IS_FLOATING_DEF(T)   typename std::enable_if<std::is_floating_point<T>::value>::type*
    #define IS_INTEGRAL_DEF(T)   typename std::enable_if<std::is_integral<T>::value>::type*
    #define IS_ARITHMETIC_DEF(T) typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type*

    //////////////////////////////////////////////////////////////////////////
    // [function traits]
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct lambda_traits;

    template <typename F, typename Ret, typename... Args>
    struct lambda_traits<Ret(F::*)(Args...) const>
    {
        static const std::size_t parameter_count = sizeof...(Args);
        using return_type = Ret;

        template <std::size_t N>
        using parameter = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    // Query function traits
    template<typename F>
    struct function_traits : lambda_traits<decltype(&F::operator())> {};

    // Function without dereferencing
    template <typename Ret, typename ...Args>
    struct function_traits<Ret(Args...)> : function_traits<Ret(*)(Args...)>
    {};

    // Functions
    template<typename Ret, typename ...Args>
    struct function_traits<Ret(*)(Args...)>
    {
        static const std::size_t parameter_count = sizeof...(Args);
        using return_type = Ret;

        template <std::size_t N>
        using parameter = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    // Methods
    template <typename C, typename Ret, typename... Args>
    struct function_traits<Ret(C::*)(Args...)>
    {
        static const std::size_t parameter_count = sizeof...(Args);
        using return_type = Ret;

        template <std::size_t N>
        using parameter = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
}