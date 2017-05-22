#include "MessageToken.h"

#include "logger.h"
#include "stl_extentions.h"

namespace pk
{
    #pragma region ctors/dtors
    MessageToken::~MessageToken()
    {
        if(_scope_exit)
        {
            unregister();
        }
    }
    #pragma endregion

    #pragma region public methods
    void MessageToken::clear()
    {
        _data.clear();
    }
    void MessageToken::unregister()
    {
        for(const auto& entry : _data)
        {
            MessageDispatcher::Unregister(entry);
        }
        _data.clear();
    }
    void MessageToken::addFilter(void * sender)
    {
        _filters.push_back(sender);
    }
    void MessageToken::clearFilters()
    {
        _filters.clear();
    }
    #pragma endregion

    #pragma region operator overload
    MessageToken& MessageToken::operator+=(MessageToken&& other)
    {
        _scope_exit &= other._scope_exit;

        for(const auto& elem : other._data)
        {
            _data.emplace_back(std::move(elem));
        }

        return *this;
    }
    #pragma endregion
}