#include "imgui_draganddrop.h"


namespace ImGui
{
    static void* _data = nullptr;
    static int _id = -1;
    static bool _highlight = false;

    void reset()
    {
        _data = nullptr;
        _id = -1;
        _highlight = false;
    }

    bool Drag(void* data, bool defaultTooltip /* = true */)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if(window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->DC.LastItemId;

        if(IsItemHovered() && g.IO.MouseClicked[0] && _data == nullptr)
        {
            _data = data;
            _id = id;
        }

        if(g.IO.MouseClicked[1])
        {
            reset();
        }

        if(_data == nullptr)
        {
            return false;
        }

        if(defaultTooltip)
        {
            if(_highlight) { ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.7f, 0.7f, 0.7f, 0.7f)); }

            ImGui::BeginTooltip();
            ImGui::Text("Dragging");
            ImGui::EndTooltip();

            if(_highlight)
            {
                ImGui::PopStyleColor();
                _highlight = false;
            }
        }
        return true;
    }
    bool Drop(void** data)
    {
        ImGuiContext& g = *GImGui;
        if(IsItemHovered() && _data != nullptr)
        {
            _highlight = true;
            if(g.IO.MouseReleased[0])
            {
                (*data) = _data;
                reset();
                return true;
            }
        }

        return false;
    }

    #pragma region engine addons
    #pragma endregion
}