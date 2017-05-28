#pragma once
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

#include <string>
#include "type_info.h"
#include "DragInfo.h"

namespace ImGui
{
    bool Drag(void* data, bool defaultTooltip = true);
    bool Drop(void** data);

    #pragma region engine addons
    template <typename T>
    bool Object(const char* label, T** result)
    {
        void* data;

        ImGui::Text((*result) == nullptr ? (std::string{label} +" [empty]").c_str() : label);
        if(ImGui::Drop(&data))
        {
            auto object = pk::DragInfo::interpret(data).get<T>();
            if(object != nullptr)
            {
                (*result) = object;
                return true;
            }
        }
        return false;
    };

    template <typename T>
    bool Object(const char* label, T* result)
    {
        void* data;

        ImGui::Text(label);
        if(ImGui::Drop(&data))
        {
            auto object = pk::DragInfo::interpret(data).get<T>();
            if(object != nullptr)
            {
                (*result) = *object;
                return true;
            }
        }
        return false;
    };

    template <typename T>
    bool Handle(T& data)
    {
        static pk::DragInfo info;
        info._type = pk::type_info<T>::type;
        info._data = &data;

        ImGui::SameLine();
        ImGui::Bullet();
        bool result = ImGui::Drag(&info);
        ImGui::NewLine();
        return result;
    }
    #pragma endregion
}