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
        if(data != nullptr)
            free(data);
    }

    void Model::create(Shader& shader, const Mesh& mesh)
    {
        create(shader, mesh);
    }
    void Model::create(const Shader& shader, const Mesh& mesh)
    {
        if(data != nullptr)
            free(data);

        const int vertex_width = shader.stride();

        const int copy_width = std::min(vertex_width, static_cast<int>(sizeof(vec3f)));
        const int data_width = vertex_width * mesh.positions().size();

        data = malloc(data_width);

        const auto& positions = mesh.positions();
        for(int i = 0; i < mesh.vertex_count(); ++i)
        {
            memcpy((char*) data + i * vertex_width, &positions[i], copy_width);
        }

        vertexBuffer = Buffer::vertex(data, data_width, vertex_width);
        indexBuffer = Buffer::index(mesh.indices().data(),
            sizeof(unsigned) * mesh.indices().size());

        count = mesh.indices().size();
    }

    void Model::draw() const
    {
        auto context = Renderer::_context();

        unsigned stride = static_cast<unsigned>(vertexBuffer.elementSize);
        unsigned offset = 0;

        context->IASetPrimitiveTopology(dx::Convert(topology));
        context->IASetVertexBuffers(0, 1, vertexBuffer.address.GetAddressOf(), &stride, &offset);
        context->IASetIndexBuffer(indexBuffer.address.Get(), DXGI_FORMAT_R32_UINT, 0);

        context->DrawIndexed(count, 0, 0);
    }
}