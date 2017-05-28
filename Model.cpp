#include "Model.h"

#ifdef MIN
#undef MIN
#endif

#include <math.h>
#include <algorithm>

#ifdef MIN
#undef MIN
#endif

#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "UtilityDX11.h"

#ifdef MIN
#undef MIN
#endif

namespace pk
{
    Model::Model(const Shader& shader, const Mesh& model)
    {
        create(shader, model);
    }
    Model::~Model(void)
    {
        if(_data != nullptr)
        {
            free(_data);
            _data = nullptr;
        }
    }

    void Model::create(const Shader& shader, const Mesh& mesh)
    {
        if(_data != nullptr)
        {
            free(_data);
            _data = nullptr;
        }

        const int vertex_width = shader.stride();

        const int copy_width = std::min(vertex_width, static_cast<int>(sizeof(vec3f)));
        const int data_width = vertex_width * mesh.positions().size();

        _data = malloc(data_width);


        const auto& positions = mesh.positions();
        const auto& texcoords = mesh.texcoords();

        for(int i = 0; i < mesh.vertex_count(); ++i)
        {
            memcpy((char*) _data + i * vertex_width, &positions[i], copy_width);
            memcpy((char*) _data + copy_width + i * vertex_width, &texcoords[i], sizeof(vec2f));
        }

        _vertexBuffer = Buffer::vertex(_data, data_width, vertex_width);
        _indexBuffer = Buffer::index(mesh.indices().data(),
            sizeof(unsigned) * mesh.indices().size());

        _count = mesh.indices().size();
    }
}