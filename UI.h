#include "imgui.h"
#include "imgui_draganddrop.h"

#include "window.h"
#include "Shader.h"
#include "Renderer.h"

#include "image.h"
#include "Texture.h"
#include "matrix4x4.h"
#include "Sampler.h"
#include "BlendState.h"
#include "DepthState.h"
#include "Rasterizer.h"

#include "MessageToken.h"

namespace pk
{
    class Window;

    namespace gui = ImGui;

    class Gui
    {
    public:
        Gui(Window& window);
        ~Gui(void);

        void refresh();
        void render(Renderer& renderer);

        void style(const std::filesystem::path& path);

    private:
        Window* _window = nullptr;

        Shader _vertexShader;
        Shader _pixelShader;

        Buffer _vertexBuffer;
        Buffer _indexBuffer;

        Texture _font;
        Sampler _sampler;
        BlendState _blend;
        DepthState _depth;
        Rasterizer _rasterizer;

        matrix4x4 _projection;

        MessageToken token;

        INT64 g_Time = 0;
        INT64 g_TicksPerSecond = 0;

        ID3D11InputLayout*       g_pInputLayout = NULL;
        ID3D11SamplerState*      g_pFontSampler = NULL;

        void createFont();

        void render(Renderer& renderer, ImDrawData* data);
        void resizeBuffers(ImDrawData* data);
        void registerInput();
    };
}