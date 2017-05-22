#pragma once
#include <vector>
#include "MessageDispatcher.h"

namespace pk
{
    class MessageToken
    {
    public:
        #pragma region ctors/dtors
        MessageToken(void) = default;

        MessageToken(bool scope_exit) : _scope_exit{scope_exit} {};
        MessageToken(MessageDispatcher::Data entry, bool scope_exit)
            : _data{entry}, _scope_exit{scope_exit}
        {};
        ~MessageToken();
        #pragma endregion

        #pragma region variables
        std::vector<MessageDispatcher::Data> _data;
        std::vector<void*> _filters;
        #pragma endregion

        #pragma region methods
        void clear();
        void unregister();

        void addFilter(void* sender);
        void clearFilters();
        #pragma endregion

        #pragma region operator overloads
        MessageToken& operator+=(MessageToken&& other);
        #pragma endregion

    private:
        #pragma region variables
        bool _scope_exit = true;
        #pragma endregion

        #pragma region friends
        friend class MessageDispatcher;
        #pragma endregion
    };
}