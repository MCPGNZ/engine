#include "UI.h"

#include "json.hpp"
#include "json_bindings.h"

#include <string>
#include <d3d11.h>
#include <d3d11shader.h>

#include <d3d10.h>
#include <d3dcompiler.h>

#include "logger.h"

#include "WindowMessages.h"
#include "MessageDispatcher.h"

#include "File.h"


namespace pk
{
    struct VERTEX_CONSTANT_BUFFER
    {
        float        mvp[4][4];
    };

    #pragma region ctors/dtors
    Gui::Gui(Window& window) :
        _blend{Blend::Alpha}
    {
        _window = &window;
        _depth.create(false, false);
        _rasterizer.create(true);

        if(!QueryPerformanceFrequency((LARGE_INTEGER *) &g_TicksPerSecond))
            return;
        if(!QueryPerformanceCounter((LARGE_INTEGER *) &g_Time))
            return;

        ImGuiIO& io = ImGui::GetIO();
        io.RenderDrawListsFn = nullptr;;
        io.ImeWindowHandle = _window->getHandle();

        io.KeyMap[ImGuiKey_Tab] = static_cast<int>(Keyboard::Key::Tab);
        io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Keyboard::Key::Left);
        io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Keyboard::Key::Right);
        io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Keyboard::Key::Up);
        io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Keyboard::Key::Down);
        io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(Keyboard::Key::PageUp);
        io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(Keyboard::Key::PageDown);
        io.KeyMap[ImGuiKey_Home] = static_cast<int>(Keyboard::Key::Home);
        io.KeyMap[ImGuiKey_End] = static_cast<int>(Keyboard::Key::End);
        io.KeyMap[ImGuiKey_Delete] = static_cast<int>(Keyboard::Key::Delete);
        io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(Keyboard::Key::BackSpace);
        io.KeyMap[ImGuiKey_Enter] = static_cast<int>(Keyboard::Key::Return);
        io.KeyMap[ImGuiKey_Escape] = static_cast<int>(Keyboard::Key::Escape);
        io.KeyMap[ImGuiKey_A] = static_cast<int>(Keyboard::Key::A);
        io.KeyMap[ImGuiKey_C] = static_cast<int>(Keyboard::Key::C);
        io.KeyMap[ImGuiKey_V] = static_cast<int>(Keyboard::Key::V);
        io.KeyMap[ImGuiKey_X] = static_cast<int>(Keyboard::Key::X);
        io.KeyMap[ImGuiKey_Y] = static_cast<int>(Keyboard::Key::Y);
        io.KeyMap[ImGuiKey_Z] = static_cast<int>(Keyboard::Key::Z);

        _vertexShader = Shader{"data/shaders/ui_vs.hlsl",{
            { ShaderLayout::Position, TextureFormat::Rg32Float, 0, 0},
            { ShaderLayout::Texcoord, TextureFormat::Rg32Float, 0, 8 },
            { ShaderLayout::Color, TextureFormat::Rgba8Unorm, 0, 16 }
        }};

        _pixelShader = Shader{"data/shaders/ui_ps.hlsl"};

        registerInput();
        style("data/settings/gui.json");
    }
    Gui::~Gui(void)
    {
        ImGui::Shutdown();
    }
    #pragma endregion

    #pragma region public methods
    void Gui::refresh()
    {
        if(!g_pFontSampler)
        {
            createFont();
        }

        ImGuiIO& io = ImGui::GetIO();

        // Setup display size (every frame to accommodate for window resizing)
        io.DisplaySize = _window->size();

        // Setup time step
        INT64 current_time;
        QueryPerformanceCounter((LARGE_INTEGER *) &current_time);
        io.DeltaTime = (float) (current_time - g_Time) / g_TicksPerSecond;
        g_Time = current_time;

        io.KeyCtrl = Keyboard::isKeyPressed(pk::Keyboard::Key::LControl);
        io.KeyShift = Keyboard::isKeyPressed(pk::Keyboard::Key::LShift);
        io.KeyAlt = Keyboard::isKeyPressed(pk::Keyboard::Key::LAlt);
        io.KeySuper = Keyboard::isKeyPressed(pk::Keyboard::Key::LSystem);

        io.MouseDown[0] = Mouse::is_button_pressed(pk::Mouse::Button::left);
        io.MouseDown[1] = Mouse::is_button_pressed(pk::Mouse::Button::right);

        if(io.MouseDrawCursor)
            SetCursor(NULL);

        ImGui::NewFrame();
    }
    void Gui::render(Renderer& renderer)
    {
        ImGui::Render();
        render(renderer, ImGui::GetDrawData());
    }

    void Gui::style(const std::filesystem::path& path)
    {
        json j = json::parse(File::readAll(path).c_str());
        auto& style = ImGui::GetStyle();

        style.WindowPadding = j["WindowPadding"].get<vec2f>();
        style.WindowRounding = j["WindowRounding"].get<float>();
        style.FramePadding = j["FramePadding"].get<vec2f>();
        style.FrameRounding = j["FrameRounding"].get<float>();
        style.ItemSpacing = j["ItemSpacing"].get<vec2f>();
        style.ItemInnerSpacing = j["ItemInnerSpacing"].get<vec2f>();
        style.IndentSpacing = j["IndentSpacing"].get<float>();
        style.ScrollbarSize = j["ScrollbarSize"].get<float>();
        style.ScrollbarRounding = j["ScrollbarRounding"].get<float>();
        style.GrabMinSize = j["GrabMinSize"].get<float>();
        style.GrabRounding = j["GrabRounding"].get<float>();

        style.Colors[ImGuiCol_Text] = j["ImGuiCol_Text"].get<vec4f>();
        style.Colors[ImGuiCol_TextDisabled] = j["ImGuiCol_TextDisabled"].get<vec4f>();
        style.Colors[ImGuiCol_WindowBg] = j["ImGuiCol_WindowBg"].get<vec4f>();
        style.Colors[ImGuiCol_ChildWindowBg] = j["ImGuiCol_ChildWindowBg"].get<vec4f>();
        style.Colors[ImGuiCol_PopupBg] = j["ImGuiCol_PopupBg"].get<vec4f>();
        style.Colors[ImGuiCol_Border] = j["ImGuiCol_Border"].get<vec4f>();
        style.Colors[ImGuiCol_BorderShadow] = j["ImGuiCol_BorderShadow"].get<vec4f>();
        style.Colors[ImGuiCol_FrameBg] = j["ImGuiCol_FrameBg"].get<vec4f>();
        style.Colors[ImGuiCol_FrameBgHovered] = j["ImGuiCol_FrameBgHovered"].get<vec4f>();
        style.Colors[ImGuiCol_FrameBgActive] = j["ImGuiCol_FrameBgActive"].get<vec4f>();
        style.Colors[ImGuiCol_TitleBg] = j["ImGuiCol_TitleBg"].get<vec4f>();
        style.Colors[ImGuiCol_TitleBgCollapsed] = j["ImGuiCol_TitleBgCollapsed"].get<vec4f>();
        style.Colors[ImGuiCol_TitleBgActive] = j["ImGuiCol_TitleBgActive"].get<vec4f>();
        style.Colors[ImGuiCol_MenuBarBg] = j["ImGuiCol_MenuBarBg"].get<vec4f>();
        style.Colors[ImGuiCol_ScrollbarBg] = j["ImGuiCol_ScrollbarBg"].get<vec4f>();
        style.Colors[ImGuiCol_ScrollbarGrab] = j["ImGuiCol_ScrollbarGrab"].get<vec4f>();
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = j["ImGuiCol_ScrollbarGrabHovered"].get<vec4f>();
        style.Colors[ImGuiCol_ScrollbarGrabActive] = j["ImGuiCol_ScrollbarGrabActive"].get<vec4f>();
        style.Colors[ImGuiCol_ComboBg] = j["ImGuiCol_ComboBg"].get<vec4f>();
        style.Colors[ImGuiCol_CheckMark] = j["ImGuiCol_CheckMark"].get<vec4f>();
        style.Colors[ImGuiCol_SliderGrab] = j["ImGuiCol_SliderGrab"].get<vec4f>();
        style.Colors[ImGuiCol_SliderGrabActive] = j["ImGuiCol_SliderGrabActive"].get<vec4f>();
        style.Colors[ImGuiCol_Button] = j["ImGuiCol_Button"].get<vec4f>();
        style.Colors[ImGuiCol_ButtonHovered] = j["ImGuiCol_ButtonHovered"].get<vec4f>();
        style.Colors[ImGuiCol_ButtonActive] = j["ImGuiCol_ButtonActive"].get<vec4f>();
        style.Colors[ImGuiCol_Header] = j["ImGuiCol_Header"].get<vec4f>();
        style.Colors[ImGuiCol_HeaderHovered] = j["ImGuiCol_HeaderHovered"].get<vec4f>();
        style.Colors[ImGuiCol_HeaderActive] = j["ImGuiCol_HeaderActive"].get<vec4f>();
        style.Colors[ImGuiCol_Column] = j["ImGuiCol_Column"].get<vec4f>();
        style.Colors[ImGuiCol_ColumnHovered] = j["ImGuiCol_ColumnHovered"].get<vec4f>();
        style.Colors[ImGuiCol_ColumnActive] = j["ImGuiCol_ColumnActive"].get<vec4f>();
        style.Colors[ImGuiCol_ResizeGrip] = j["ImGuiCol_ResizeGrip"].get<vec4f>();
        style.Colors[ImGuiCol_ResizeGripHovered] = j["ImGuiCol_ResizeGripHovered"].get<vec4f>();
        style.Colors[ImGuiCol_ResizeGripActive] = j["ImGuiCol_ResizeGripActive"].get<vec4f>();
        style.Colors[ImGuiCol_CloseButton] = j["ImGuiCol_CloseButton"].get<vec4f>();
        style.Colors[ImGuiCol_CloseButtonHovered] = j["ImGuiCol_CloseButtonHovered"].get<vec4f>();
        style.Colors[ImGuiCol_CloseButtonActive] = j["ImGuiCol_CloseButtonActive"].get<vec4f>();
        style.Colors[ImGuiCol_PlotLines] = j["ImGuiCol_PlotLines"].get<vec4f>();
        style.Colors[ImGuiCol_PlotLinesHovered] = j["ImGuiCol_PlotLinesHovered"].get<vec4f>();
        style.Colors[ImGuiCol_PlotHistogram] = j["ImGuiCol_PlotHistogram"].get<vec4f>();
        style.Colors[ImGuiCol_PlotHistogramHovered] = j["ImGuiCol_PlotHistogramHovered"].get<vec4f>();
        style.Colors[ImGuiCol_TextSelectedBg] = j["ImGuiCol_TextSelectedBg"].get<vec4f>();
        style.Colors[ImGuiCol_ModalWindowDarkening] = j["ImGuiCol_ModalWindowDarkening"].get<vec4f>();
    }
    #pragma endregion

    #pragma region private methods
    void Gui::render(Renderer& renderer, ImDrawData* data)
    {
        resizeBuffers(data);

        ID3D11DeviceContext* ctx = Renderer::_context();

        // Copy and convert all vertices into a single contiguous buffer
        ImDrawVert* vtx_dst = reinterpret_cast<ImDrawVert*>(_vertexBuffer.lock());
        ImDrawIdx* idx_dst = reinterpret_cast<ImDrawIdx*>(_indexBuffer.lock());
        for(int n = 0; n < data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = data->CmdLists[n];
            memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

            vtx_dst += cmd_list->VtxBuffer.Size;
            idx_dst += cmd_list->IdxBuffer.Size;
        }
        _vertexBuffer.unlock();
        _indexBuffer.unlock();

        {
            auto& io = ImGui::GetIO();

            float L = 0.0f;
            float R = io.DisplaySize.x;
            float B = io.DisplaySize.y;
            float T = 0.0f;
            float mvp[4][4] =
            {
                { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
                { 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
                { 0.0f,         0.0f,           0.5f,       0.0f },
                { (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
            };

            _vertexShader.set("ProjectionMatrix", &mvp);
        }

        renderer.set(_vertexBuffer);
        renderer.set(_indexBuffer);

        renderer.set(_vertexShader);
        renderer.set(_pixelShader);

        renderer.set(_sampler);
        renderer.set(_blend);
        _depth.bind(renderer);
        _rasterizer.bind(renderer);

        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Render command lists
        int vtx_offset = 0;
        int idx_offset = 0;
        for(int n = 0; n < data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = data->CmdLists[n];
            for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if(pcmd->UserCallback)
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
                else
                {
                    const D3D11_RECT r = {(LONG) pcmd->ClipRect.x, (LONG) pcmd->ClipRect.y, (LONG) pcmd->ClipRect.z, (LONG) pcmd->ClipRect.w};
                    ctx->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**) &pcmd->TextureId);
                    ctx->RSSetScissorRects(1, &r);
                    ctx->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
                }
                idx_offset += pcmd->ElemCount;
            }
            vtx_offset += cmd_list->VtxBuffer.Size;
        }
    }

    void Gui::createFont()
    {
        ImGuiIO& io = ImGui::GetIO();
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        _font = Texture{{vec2i{width, height}, pixels}};
        io.Fonts->TexID = _font.id();
    }

    void Gui::resizeBuffers(ImDrawData* data)
    {
        if(_vertexBuffer.count() < data->TotalVtxCount || _vertexBuffer.empty())
        {
            _vertexBuffer.create(nullptr, (data->TotalVtxCount + 5000) * sizeof(ImDrawVert), sizeof(ImDrawVert),
                pk::BufferBind::Vertex, pk::BufferUsage::Dynamic);
        }
        if(_indexBuffer.count() < data->TotalIdxCount || _indexBuffer.empty())
        {
            _indexBuffer.create(nullptr, (data->TotalIdxCount + 10000) * sizeof(ImDrawIdx), sizeof(ImDrawIdx),
                pk::BufferBind::Index, pk::BufferUsage::Dynamic);
        }
    }
    void Gui::registerInput()
    {
        using namespace messages::window;

        token.addFilter(_window);

        token += MessageDispatcher::Register([&](const mouseMoved& mouse){
            ImGui::GetIO().MousePos = mouse.position;
        });

        token += MessageDispatcher::Register([](const keyDown& button){
            ImGui::GetIO().KeysDown[(int) button.key] = true;
        });

        token += MessageDispatcher::Register([](const keyUp& button){
            ImGui::GetIO().KeysDown[(int) button.key] = false;
        });

        token += MessageDispatcher::Register([](const ascii& button){
            ImGui::GetIO().AddInputCharacter((unsigned short) button.index);
        });
    }
    #pragma endregion
}