#pragma once
#include <type_traits>
#include <vector>
#include <functional>

#include "traits.h"
#include "type_info.h"

#include "stl_extentions.h"

namespace pk
{
    class MessageToken;

    class MessageDispatcher
    {
        #pragma region Forward declarations
        struct Data;
        #pragma endregion

    public:
        #pragma region Public Methods
        template <typename F, typename T>
        static MessageToken Register(const F& method, T* obj);

        template <typename F, typename T>
        static MessageToken Register(const F& method, T& obj);

        template <typename F>
        static MessageToken Register(F& function);

        template <typename F>
        static MessageToken Register(const F& function);

        template <typename F, typename T>
        static void Unregister(const F& method, T* obj);

        template <typename T>
        static void Unregister(T* obj, IS_FALSE(std::is_function<T>));

        template <typename T>
        static void Unregister(T& obj, IS_FALSE(std::is_function<T>));

        template <typename T>
        static void Unregister(T* obj, IS_TRUE(std::is_function<T>));

        static void Unregister(const Data& obj);

        template <typename T>
        static void Signal(T&& message, void* sender);
        #pragma endregion

    private:
        #pragma region Private types
        struct Data
        {
            #pragma region Operator overloads
            bool operator==(const Data& other) const noexcept
            {
                return messageType == other.messageType &&
                    functor_pointer == other.functor_pointer &&
                    receiver == other.receiver;
            }
            bool operator==(Data&& other) const noexcept
            {
                return messageType == other.messageType &&
                    functor_pointer == other.functor_pointer &&
                    receiver == other.receiver;
            }
            #pragma endregion

            #pragma region Public variables
            std::function<void(void*)> method;
            pk::type messageType;
            void* functor_pointer;
            void* receiver;
            MessageToken* token = nullptr;
            #pragma endregion
        };
        #pragma endregion

        #pragma region Private variables
        static std::vector<Data> bindings;
        #pragma endregion

        #pragma region Helper methods
        static void UnregisterFunction(void* obj);
        static void UnregisterObject(void* obj);

        static Data& Find(const Data& data);
        #pragma endregion

        #pragma region Friends
        friend class MessageToken;
        #pragma endregion
    };

    #pragma region Public templates

    #pragma region Register
    template <typename F, typename T>
    MessageToken MessageDispatcher::Register(const F& method, T* obj)
    {
        using MessageType = std::decay_t<pk::function_traits<F>::parameter<0>>;

        const auto lambda = [=](void* object){
            auto message = *reinterpret_cast<MessageType*>(object);
            (obj->*method)(message); };

        Data data;
        data.method = lambda;
        data.messageType = pk::type_info<MessageType>::type;
        data.receiver = obj;
        data.functor_pointer = (void*&) method;

        bindings.emplace_back(std::move(data));
        return MessageToken{bindings.back(), false};
    }

    template <typename F, typename T>
    MessageToken MessageDispatcher::Register(const F& method, T& obj)
    {
        return Register(method, &obj);
    }

    template <typename F>
    MessageToken MessageDispatcher::Register(F& function)
    {
        using MessageType = std::decay_t<pk::function_traits<F>::parameter<0>>;

        const auto lambda = [=](const void* object){
            const auto message = *reinterpret_cast<const MessageType*>(object);
            function(message); };

        Data data;
        data.method = lambda;
        data.messageType = pk::type_info<MessageType>::type;
        data.receiver = &function;
        data.functor_pointer = (void*&) function;

        bindings.emplace_back(std::move(data));
        return MessageToken{bindings.back(), false};
    }

    template <typename F>
    MessageToken MessageDispatcher::Register(const F& function)
    {
        using MessageType = std::decay_t<pk::function_traits<F>::parameter<0>>;

        const auto lambda = [=](const void* object){
            const auto message = *reinterpret_cast<const MessageType*>(object);
            function(message); };

        Data data;
        data.method = lambda;
        data.messageType = pk::type_info<MessageType>::type;
        data.receiver = nullptr;
        data.functor_pointer = (void*&) function;

        bindings.emplace_back(std::move(data));
        return MessageToken{bindings.back(), false};
    }
    #pragma endregion

    #pragma region Unregister
    template <typename F, typename T>
    void MessageDispatcher::Unregister(const F& method, T* obj)
    {
        for(size_t i = 0; i < bindings.size();)
        {
            const auto& data = bindings[i];
            if(data.receiver == obj && data.functor_pointer == (void*&) method)
            {
                std::swap(bindings[i], bindings.back());
                bindings.pop_back();
                continue;
            }
            ++i;
        }
    }

    template <typename T>
    static void MessageDispatcher::Unregister(T* obj, IS_FALSE_DEF(std::is_function<T>))
    {
        UnregisterObject(obj);
    }

    template <typename T>
    static void MessageDispatcher::Unregister(T& obj, IS_FALSE_DEF(std::is_function<T>))
    {
        UnregisterObject(&obj);
    }

    template <typename T>
    static void MessageDispatcher::Unregister(T* obj, IS_TRUE_DEF(std::is_function<T>))
    {
        UnregisterFunction(obj);
    }
    #pragma endregion

    #pragma region Signal
    template <typename T>
    void MessageDispatcher::Signal(T&& message, void* sender /* = nullptr */)
    {
        for(size_t i = 0; i < bindings.size(); ++i)
        {
            const auto& bind = bindings[i];
            const auto& messageType = pk::type_info<T>::type;

            if(bind.messageType != messageType)
            {
                continue;
            }

            bind.method((void*) &message);
        }
    }
    #pragma endregion
    #pragma endregion
}