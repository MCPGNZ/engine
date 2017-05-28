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
    public:
        Model(void) = default;
        Model(const Shader& shader, const Mesh& mesh);
        ~Model(void);

        Model(const Model& other) = delete;
        Model& operator=(const Model& other) = delete;

        Model(Model&&) = default;
        Model& operator=(Model&&) = default;

        void create(const Shader& shader, const Mesh& mesh);

    private:
        void* _data = nullptr;

        Buffer _vertexBuffer;
        Buffer _indexBuffer;
        int _count = 0;

        Topology _topology = Topology::TriangleList;

        friend class Renderer;
    };
}