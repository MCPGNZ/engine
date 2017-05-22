#pragma once
#include <vector>

#include "Buffer.h"
#include "GPUDesc.h"

namespace pk
{
    class Mesh;
    class Shader;
}

namespace pk
{
    class Model
    {
    private:
        void* data = nullptr;

        Buffer vertexBuffer;
        Buffer indexBuffer;
        int count = 0;

        Topology topology = Topology::TriangleList;

        friend class Graphics;
    public:
        Model(void) = default;
        Model(const Shader& shader, const Mesh& mesh);
        ~Model(void);

        Model(const Model& other) = delete;
        Model& operator=(const Model& other) = delete;

        Model(Model&&) = default;
        Model& operator=(Model&&) = default;

        void create(Shader& shader, const Mesh& mesh);
        void create(const Shader& shader, const Mesh& mesh);

        void draw() const;
    };
}