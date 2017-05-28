#include "SpriteSystem.h"

#include <future>

#include "Renderer.h"
#include "Resources.h"
#include "vec3.h"
#include "stl_extentions.h"
#include "image.h"

namespace pk
{
    #pragma region static variables
    Mesh SpriteSystem::_mesh = Mesh::Quad();
    Texture SpriteSystem::_texture;
    #pragma endregion

    #pragma region ctors/dtors
    SpriteSystem::SpriteSystem(void)
    {
        static Shader defaultVertexShader = Shader{"data/shaders/sprite_vs.hlsl"};
        static Shader defaultPixelShader = Shader{"data/shaders/sprite_ps.hlsl"};

        _vertexShader = defaultVertexShader;
        _pixelShader = defaultPixelShader;

        _blend = BlendState{pk::Blend::Additive};
        _model = Model{_vertexShader, _mesh};

        _depth.create(true);
        _rasterizer.create(false);
    }

    SpriteSystem::SpriteSystem(Resources& resources) : SpriteSystem()
    {
        _resources = &resources;
    }
    #pragma endregion

    #pragma region public methods
    int SpriteSystem::add()
    {
        auto sprite = Sprite{*this};
        auto transform = Transform{};
        transform.position = vec3f{0.0f, 0.0f, 0.0f};

        _sprites.push_back(sprite);
        _transforms.push_back(transform);

        return std::lastIndex(_sprites);
    }

    Sprite& SpriteSystem::sprite(const int id)
    {
        return _sprites[id];
    }
    Transform& SpriteSystem::transform(const int id)
    {
        return _transforms[id];
    }

    void SpriteSystem::vertexShader(Shader shader)
    {
        _vertexShader = shader;
    }
    void SpriteSystem::pixelShader(Shader shader)
    {
        _pixelShader = shader;
    }

    void SpriteSystem::renderAll(Renderer& renderer)
    {
        for(auto& system : _components)
        {
            system.render(renderer);
        }
    }

    void SpriteSystem::render(Renderer& renderer)
    {
        if(_resources == nullptr)
        {
            return;
        }

        renderer.set(_vertexShader);
        renderer.set(_pixelShader);

        renderer.set(_model);
        renderer.set(_blend);

        _depth.bind(renderer);
        _rasterizer.bind(renderer);

        int _currentTextureId = std::numeric_limits<int>::min();

        const auto length = _sprites.size();
        for(int i = 0; i < length; ++i)
        {
            const auto& sprite = _sprites[i];
            const auto& transform = _transforms[i];

            if(sprite._texture != _currentTextureId)
            {
                if(sprite._texture >= _resources->_textures.size())
                {
                    _texture = Texture{Image{vec2i{32}, Color::default}};
                    _pixelShader.set("diffuse", &_texture);
                }
                else
                {
                    _currentTextureId = sprite._texture;
                    _pixelShader.set("diffuse", &_resources->_textures[sprite._texture]);
                }
            }
            _vertexShader.set("world", &math::transpose(_transforms[i].worldMatrix()));

            renderer.draw(_model);
        }
    }
    #pragma endregion
}