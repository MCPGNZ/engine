#include <vld.h>

#include <vector>
#include <initializer_list>

#include "vec2.h"
#include "matrix4x4.h"

#include "MessageToken.h"
#include "MessageDispatcher.h"

#include "window.h"
#include "image.h"

#include "AudioWAV.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "FFT.h"

#include "FileWatcher.h"

#include "Renderer.h"
#include "RenderTarget.h"

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Viewport.h"

#include "Camera.h"
#include "Transform.h"

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

    pk::Mesh  mesh{"data/meshes/teapot.fbx"};
    pk::Model model{vertex_shader, mesh};
    pk::Transform transform{};

    pk::Camera camera{};

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
        window.broadcastEvents();

        vertex_shader.set("view", &pk::math::transpose(camera.view()));
        vertex_shader.set("projection", &pk::math::transpose(camera.projection()));

        renderer.clear(pk::Color{0.1f, 0.1f, 0.1f});
        {
            static float angle = 0.0f;
            angle += 0.01f;

            transform.position.x = 10.1f;
            transform.rotation.z = angle;

            vertex_shader.set("world", &pk::math::transpose(transform.world()));

            model.draw();
        }
        renderer.present();
    }
}