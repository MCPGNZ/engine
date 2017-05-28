#include "Mesh.h"

#include "logger.h"
#include "degree.h"
#include "radian.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace pk
{
    #pragma region Ctors
    Mesh::Mesh(const std::filesystem::path& path)
    {
        load(path);
    }
    #pragma endregion

    #pragma region Public methods
    void Mesh::clear()
    {
        _indices.clear();
        _vertices.clear();
    }
    void Mesh::load(const std::filesystem::path& path)
    {
        clear();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string(),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_ValidateDataStructure |
            aiProcess_ImproveCacheLocality |
            aiProcess_ConvertToLeftHanded);

        logger::assert_cond(scene != nullptr, "Mesh", "load", ("could not load file: " + path.string()).c_str());
        logger::assert_cond(scene->mNumMeshes != 0, "Mesh", "load", "no meshes found in loaded file");

        aiMesh* mesh = scene->mMeshes[0];

        const auto vertexCount = mesh->mNumVertices;
        const auto indexCount = mesh->mNumFaces * 4;

        _vertices.resize(vertexCount);

        for(int i = 0; i < vertexCount; ++i)
        {
            _vertices[i].position = vec3f::cast(mesh->mVertices[i]);
            _vertices[i].normal = vec3f::cast(mesh->mNormals[i]);
            _vertices[i].texcoord = vec2f::cast(mesh->mTextureCoords[0][i]);
        }

        _indices.resize(indexCount);
        for(int i = 0; i < indexCount / 4; ++i)
        {
            _indices[4 * i + 0] = mesh->mFaces[i].mIndices[0];
            _indices[4 * i + 1] = mesh->mFaces[i].mIndices[1];
            _indices[4 * i + 2] = mesh->mFaces[i].mIndices[2];
            _indices[4 * i + 3] = 0xffffffff;
        }
    }

    int Mesh::vertex_count() const
    {
        return _vertices.size();
    }

    std::vector<vec3f> Mesh::positions() const
    {
        std::vector<vec3f> result;
        result.reserve(_vertices.size());

        for(const auto& v : _vertices)
        {
            result.push_back(v.position);
        }

        return result;
    }
    std::vector<vec3f> Mesh::normals() const
    {
        std::vector<vec3f> result;
        result.reserve(_vertices.size());

        for(const auto& v : _vertices)
        {
            result.push_back(v.normal);
        }

        return result;
    }
    std::vector<vec2f> Mesh::texcoords() const
    {
        std::vector<vec2f> result;
        result.reserve(_vertices.size());

        for(const auto& v : _vertices)
        {
            result.push_back(v.texcoord);
        }

        return result;
    }

    std::vector<unsigned> Mesh::indices()
    {
        return _indices;
    }
    const std::vector<unsigned> Mesh::indices() const
    {
        return _indices;
    }
    #pragma endregion

    #pragma region Statics
    Mesh Mesh::Triangle()
    {
        Mesh result;
        result._vertices.resize(3);

        result._vertices[0].position = {-0.5f, -0.5f, 0.0f};
        result._vertices[0].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[0].texcoord = {0.0f, 1.0f};

        result._vertices[1].position = {0.0f, 0.5f, 0.0f};
        result._vertices[1].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[1].texcoord = {0.5f, 0.0f};

        result._vertices[2].position = {0.5f, -0.5f, 0.0f};
        result._vertices[2].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[2].texcoord = {1.0f, 1.0f};

        result._indices = {0, 1, 2};

        return result;
    }
    Mesh Mesh::Quad()
    {
        Mesh result;
        result._vertices.resize(4);

        result._vertices[0].position = {-0.5f,  -0.5f, 0.0f};
        result._vertices[0].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[0].texcoord = {0.0f, 1.0f};

        result._vertices[1].position = {-0.5f,  +0.5f, 0.0f};
        result._vertices[1].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[1].texcoord = {0.0f, 0.0f};

        result._vertices[2].position = {+0.5f,  -0.5f, 0.0f};
        result._vertices[2].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[2].texcoord = {1.0f, 1.0f};

        result._vertices[3].position = {+0.5f,  +0.5f, 0.0f};
        result._vertices[3].normal = {0.0f, 0.0f, -1.0f};
        result._vertices[3].texcoord = {1.0f, 0.0f};

        result._indices = {0, 1, 2, 1, 2, 3};

        return result;
    }

    Mesh Mesh::Circle(const int sides /* = 12 */, const float diameter /* = 1.0f */)
    {
        Mesh result;
        result._vertices.resize(sides * 3);
        result._indices.resize(sides * 4);

        const radian rad = degree{360.0f / static_cast<float>(sides)};
        const float angle = rad.value;

        const float halfDiameter = diameter * 0.5f;

        for(int i = 0; i < sides * 3; i += 3)
        {
            result._vertices[i + 0].position = {0.0f, 0.0f, 0.0f};
            result._vertices[i + 0].texcoord = {0.5f, 0.5f};
            result._vertices[i + 0].normal = {0.0f, 0.0f, -1.0f};

            result._vertices[i + 1].position = halfDiameter * vec3f{cos(angle*(i / 3 + 1)), sin(angle*(i / 3 + 1)), 0.0f};
            result._vertices[i + 1].texcoord = (vec2f{cos(angle*(i / 3 + 1)), -sin(angle*(i / 3 + 1))} +1.0f) / 2.0f;
            result._vertices[i + 1].normal = {0.0f, 0.0f, -1.0f};

            result._vertices[i + 2].position = halfDiameter * vec3f{cos(angle*i / 3), sin(angle*i / 3), 0.0f};
            result._vertices[i + 2].texcoord = (vec2f{cos(angle*i / 3), -sin(angle*i / 3)} +1.0f) / 2.0f;
            result._vertices[i + 2].normal = {0.0f, 0.0f, -1.0f};
        }

        for(int i = 0; i < sides * 3; i += 3)
        {
            const int index = 4 * i / 3;

            result._indices[index] = i;
            result._indices[index + 1] = i + 1;
            result._indices[index + 2] = i + 2;
            result._indices[index + 3] = 0xffffffff;
        }

        return result;
    }

    #pragma endregion
}