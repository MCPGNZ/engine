#pragma once
#include <vector>

#include "File.h"
#include "vec2.h"
#include "vec3.h"

namespace pk
{
    class Mesh
    {
    public:
        #pragma region Public types
        struct VertexData
        {
            vec3f position;
            vec3f normal;
            vec2f texcoord;
        };
        #pragma endregion

        #pragma region Ctors
        Mesh(void) = default;
        Mesh(const std::filesystem::path& path);
        #pragma endregion

        #pragma region Public methods
        void load(const std::filesystem::path& path);
        void clear();

        int vertex_count() const;

        std::vector<vec3f> positions() const;
        std::vector<vec3f> normals()   const;
        std::vector<vec2f> texcoords() const;

        std::vector<unsigned> indices();
        const std::vector<unsigned> indices() const;
        #pragma endregion

        #pragma region Statics
        static Mesh Triangle();
        static Mesh Quad();
        static Mesh Circle(const int sides = 12, const float diameter = 1.0f);
        #pragma endregion

    private:
        #pragma region Private variables
        std::vector<unsigned> _indices;
        std::vector<VertexData> _vertices;
        #pragma endregion
    };
}
