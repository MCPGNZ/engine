#pragma region includes
#include <vld.h>

#include <vector>
#include <iostream>
#include <initializer_list>

#include "Console.h"
#include "ConsoleView.h"

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

#include "Timer.h"

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Viewport.h"

#include "Camera.h"
#include "Transform.h"

#include "UI.h"

#include "Sprite.h"
#include "SpriteSystem.h"
#include "SpriteSystemView.h"

#include "Resources.h"
#include "ResourcesView.h"

#include "Vehicle.h"

#include "LifecycleSystem.h"

#include "ApplicationView.h"
#include "Space.h"
#include "SpaceView.h"
#include "SpriteView.h"

#include "stb_image.h"
#include "json.hpp"
#include "json_bindings.h"

#include "View.h"
#include "desktop.h"

#pragma endregion

const auto title = "pk::engine";
const auto ps_path = "data/shaders/pixel_shader.ps";
const auto vs_path = "data/shaders/vertex_shader.vs";

void setup()
{
    pk::SpaceViewEntry resourcesEntry;
    resourcesEntry._name = "resources";
    resourcesEntry._type = pk::type_info<pk::Resources>::type;
    resourcesEntry._menu = [&](pk::Space& space){
        auto id = pk::Resources::create();
        space.add(id, "resources");
    };
    resourcesEntry._onClick = [](){
        pk::View::_current->_function = [](){
            pk::ResourcesView::render(pk::Resources::_components[0]);
        };
    };


    pk::SpaceViewEntry spriteSystemEntry;
    spriteSystemEntry._name = "sprite system";
    spriteSystemEntry._type = pk::type_info<pk::SpriteSystem>::type;
    spriteSystemEntry._menu = [&](pk::Space& space){
        auto id = pk::SpriteSystem::create();
        space.add(id, "sprite system");
    };
    spriteSystemEntry._onClick = [](){
        pk::View::_current->_function = [](){
            pk::SpriteSystemView::render(pk::SpriteSystem::_components[0]);
        };
    };

    pk::SpaceView::add("resources", resourcesEntry);
    pk::SpaceView::add("sprite system", spriteSystemEntry);
}

int main()
{
    setup();

    pk::Console console{};
    pk::ConsoleView consoleView{console};

    pk::Space space{};
    pk::SpaceView spaceView{space};

    pk::View view{};

    pk::Desktop desktop{};

    pk::Window window{desktop.resolution() * 0.9f, title};
    window.center();

    pk::Renderer renderer{};
    pk::RenderTarget target{window.getHandle(), window.size()};
    pk::Viewport viewport{{0.0f},{1.0f}};

    pk::Camera camera{};
    pk::Timer timer{};

    pk::Gui gui{window};

    pk::LifecycleSystem lifecycleSystem{};
    pk::ApplicationView applicationView{};

    renderer.set(target);
    renderer.set(viewport);

    camera.perspective(pk::vec2i{32, (int) (32 * window.aspect())}, 45.0_deg, 0.1f, 200.0f);
    camera.origin({0.0f, 0.0f, -100.0f});
    camera.direction({0.0f, 0.0f, 1.0f});

    pk::SpriteSystem spriteSystem{};
    auto& vertexShader = spriteSystem.vertexShader();

    auto bcgId = spriteSystem.add();
    auto& sprite = spriteSystem.sprite(bcgId);
    auto& transform = spriteSystem.transform(bcgId);

    sprite.texture(0);
    transform.scale = pk::vec3f{100.0f, 100.0f, 1.0f};
    transform.position.z = 1.0f;

    pk::Vehicle vehicle{spriteSystem};
    pk::SpriteView spriteView{vehicle._sprite, spriteSystem};

    bool done = false;

    while(!done)
    {
        auto deltaTime = timer.delta();
        gui.refresh();

        window.broadcastEvents();
        vehicle.update(deltaTime);

        lifecycleSystem.update(deltaTime);
        applicationView.render();

        auto pos = vehicle.position();
        camera.origin(pk::vec3f{pos.x, pos.y, -100.0f});
        camera.direction({0.0f, 0.0f, 1.0f});
        vertexShader.set("view", &pk::math::transpose(camera.view()));
        vertexShader.set("projection", &pk::math::transpose(camera.projection()));

        renderer.clear(pk::Color{0.1f, 0.1f, 0.1f});
        {
            pk::SpriteSystem::renderAll(renderer);
            gui.render(renderer);
        }
        renderer.present();
    }
}