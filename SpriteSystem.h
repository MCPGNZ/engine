#pragma once
#include <vector>
#include "Sprite.h"

#include "Mesh.h"
#include "Model.h"
#include "Shader.h"

#include "Texture.h"
#include "Transform.h"

#include "BlendState.h"
#include "DepthState.h"
#include "Rasterizer.h"
#include "Data.h"

namespace pk
{
    class Renderer;
    class Resources;

    class SpriteSystem : public Data<SpriteSystem>
    {
    public:
        SpriteSystem(void);
        SpriteSystem(Resources& resources);

        int add();
        Sprite& sprite(const int id);
        Transform& transform(const int id);

        Shader& vertexShader() { return _vertexShader; };
        Shader& pixelShader() { return _pixelShader; };

        void vertexShader(Shader shader);
        void pixelShader(Shader shader);

        void render(Renderer& renderer);

        static void renderAll(Renderer& renderer);

    private:
        std::vector<Sprite> _sprites;
        std::vector<Transform> _transforms;

        Resources* _resources = nullptr;

        BlendState _blend;
        DepthState _depth;
        Rasterizer _rasterizer;

        Model _model;

        Shader _vertexShader;
        Shader _pixelShader;

        static Mesh _mesh;
        static Texture _texture;

        friend class Sprite;
        friend class SpriteSystemView;
    };
}