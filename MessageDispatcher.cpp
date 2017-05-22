#include "MessageDispatcher.h"

#include "logger.h"
#include "stl_extentions.h"

namespace pk
{
    #pragma region Statics
    std::vector<MessageDispatcher::Data> MessageDispatcher::bindings;
    #pragma endregion

    #pragma region Helper methods
    void MessageDispatcher::UnregisterObject(void* obj)
    {
        for(size_t i = 0; i < bindings.size();)
        {
            const auto& data = bindings[i];
            if(data.receiver == obj)
            {
                std::swap(bindings[i], bindings.back());
                bindings.pop_back();
                continue;
            }
            ++i;
        }
    }
    void MessageDispatcher::UnregisterFunction(void* obj)
    {
        for(size_t i = 0; i < bindings.size(); ++i)
        {
            const auto& data = bindings[i];
            if(data.functor_pointer == obj)
            {
                std::swap(bindings[i], bindings.back());
                bindings.pop_back();
                return;
            }
        }
    }
    void MessageDispatcher::Unregister(const Data& data)
    {
        auto& it = std::find(bindings.begin(), bindings.end(), data);
        if(it == bindings.end())
        {
            return;
        }

        std::swap(*it, bindings.back());
        bindings.pop_back();
    }

    MessageDispatcher::Data& MessageDispatcher::Find(const Data& data)
    {
        auto element = std::find(bindings, data);
        logger::assert_cond(element != nullptr, "MessageDispatcher", "Find", "element == nullptr");
        return *element;
    }
    #pragma endregion
}