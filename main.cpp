#include <vld.h>

#include <vector>
#include <initializer_list>
#include "vec2.h"
#include "matrix4x4.h"

#include "window.h"
#include "image.h"
#include "imgui.h"

#include "Renderer.h"
#include "RenderTarget.h"

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Viewport.h"

#include "Camera.h"

#include "AudioWAV.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "FFT.h"

#include "FileWatcher.h"

#include "MessageToken.h"
#include "MessageDispatcher.h"

#include "UI.h"

#define Listen(message, function) pk::MessageDispatcher::Register([&](message msg){function;});

void render(pk::Renderer& renderer, pk::Shader& vs, pk::Shader& ps,
    std::vector<pk::Model*> models, std::vector<float>& spectrum)
{
    renderer.clear(pk::Color{0.1f, 0.1f, 0.1f});

    static float angle = 0.0f;

    if(spectrum.size() > 0)
    {
        angle -= spectrum[spectrum.size() / 5.0f] * 40.0f;

        std::vector<std::vector<pk::Color>> colors;
        colors.emplace_back();
        for(const auto& value : spectrum)
        {
            colors[0].push_back(pk::Color{value, value, value, 1.0f});
        }
        pk::Image image{colors};
        pk::Texture texture{image};

        ps.set("spectrum", &texture);
    }

    renderer.set(vs);
    renderer.set(ps);
    for(const auto& model : models)
    {
        const auto world = pk::math::rotation_x(pk::degree{angle});
        vs.set("world", &pk::math::transpose(world));

        model->draw();
    }

    renderer.present();
}

const auto title = "pk::engine";
const auto ps_path = "data/shaders/pixel_shader.ps";
const auto vs_path = "data/shaders/vertex_shader.vs";

int main()
{
    pk::Window window{1024, title};

    pk::Renderer renderer{};
    pk::RenderTarget target{window.getHandle(), window.size()};
    pk::Viewport viewport{{0.0f},{1.0f}};

    pk::Shader pixel_shader{ps_path};
    pk::Shader vertex_shader{vs_path};

    pk::FileWatcher pixel_watcher{ps_path,  [&](){ pixel_shader.reload();  }};
    pk::FileWatcher vertex_watcher{vs_path, [&](){ vertex_shader.reload(); }};

    pk::Mesh  mesh{"data/meshes/teapot.fbx"};
    pk::Model model{vertex_shader, mesh};

    pk::Camera camera{};

    pk::SoundSystem audio{};
    pk::Sound sound{"data/music/audio.flac"};

    pk::FFT fft{*sound.channel()};

    renderer.set(target);
    renderer.set(viewport);

    renderer.set(vertex_shader);
    renderer.set(pixel_shader);

    camera.perspective(pk::vec2i{32}, 45.0_deg, 0.1f, 200.0f);
    camera.origin({0.0f, 0.0f, -100.0f});
    camera.direction({0.0f, 0.0f, 1.0f});

    bool done = false;
    while(!done)
    {
        pixel_watcher.update();
        vertex_watcher.update();

        audio.update();

        vertex_shader.set("view", &pk::math::transpose(camera.view()));
        vertex_shader.set("projection", &pk::math::transpose(camera.projection()));

        window.handleEvents([](const pk::Event& e){});

        render(renderer, vertex_shader, pixel_shader,
            std::vector<pk::Model*>{&model}, fft.values());
    }
}



/*#include <vld.h>

#include <vector>
#include <initializer_list>
#include "vec2.h"
#include "matrix4x4.h"
#include "logger.h"

#include "window.h"
#include "image.h"

#include "Renderer.h"
#include "RenderTarget.h"

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Viewport.h"

#include "UI.h"

#include "MessageToken.h"
#include "MessageDispatcher.h"
#include "WindowMessages.h"
#include "Inspector.h"

#include "Loader.h"
#include "json.hpp"

#include "Camera.h"
#include "Transform.h"

const auto ps_path = "data/shaders/pixel_shader.ps";
const auto vs_path = "data/shaders/vertex_shader.vs";

int main()
{
    pk::Window window{1024, "pk::engine"};

    pk::Renderer renderer{};
    pk::RenderTarget target{window.getHandle(), window.size()};
    pk::Viewport viewport{{0.0f},{1.0f}};

    pk::Shader pixel_shader{ps_path};
    pk::Shader vertex_shader{vs_path};

    pk::Mesh  mesh{"data/meshes/teapot.fbx"};
    pk::Model model{vertex_shader, mesh};
    pk::Camera camera{};

    renderer.set(target);
    renderer.set(viewport);

    renderer.set(vertex_shader);
    renderer.set(pixel_shader);

    camera.perspective(pk::vec2i{32}, 45.0_deg, 0.1f, 200.0f);
    camera.origin({0.0f, 0.0f, -100.0f});
    camera.direction({0.0f, 0.0f, 1.0f});

    bool done = false;
    float angle = 0.0f;

    while(!done)
    {
        vertex_shader.set("view", &pk::math::transpose(camera.view()));
        vertex_shader.set("projection", &pk::math::transpose(camera.projection()));

        window.broadcastEvents();

        angle += 0.1f;
        renderer.clear(pk::Color{0.1f, 0.1f, 0.1f});
        {
            const auto world = pk::math::rotation_x(pk::degree{angle});
            vertex_shader.set("world", &pk::math::transpose(world));

            model.draw();
        }
        renderer.present();
    }
}
*/