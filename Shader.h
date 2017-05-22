#pragma once
#include <string>
#include <future>
#include <unordered_map>

#include <wrl\client.h>

#include <d3dcommon.h>
#include <d3d11.h>
#include <d3d11shader.h>

#include "Filesystem.h"
#include "Buffer.h"

namespace pk
{
    #pragma region Usings
    using Microsoft::WRL::ComPtr;
    #pragma endregion

    class Renderer;

    class Shader
    {
    public:
        struct Layout
        {
            ShaderLayout layout;
            TextureFormat format;
            int index;
            int offset = -1;
        };
        enum struct Type
        {
            PixelShader,
            VertexShader
        };

        #pragma region Ctors
        Shader(void) = default;
        Shader(const std::filesystem::path& path, std::vector<Layout> input = {});
        #pragma endregion

        #pragma region Public methods
        int stride() const;

        void load(const std::filesystem::path& path, std::vector<Layout> input = {});
        void reload();

        bool exists(const std::string& name) const;

        Type type() const;

        //[todo] refactor
        bool isVS(const std::filesystem::path& path);
        bool isPS(const std::filesystem::path& path);

        void set(const std::string& name, void* data);
        void* get(const std::string& name);
        #pragma endregion

        #pragma region Friends
        friend class ShaderView;
        friend class Renderer;
        #pragma endregion
        ComPtr<ID3D11InputLayout>  _layout;
    private:
        #pragma region Private types
        struct ConstantBufferData
        {
            int index;

            Buffer buffer;
            std::vector<byte> bytes;
        };
        struct ConstantBufferVariable
        {
            int bufferIndex;
            std::string name;

            int offset;
            int size;
        };
        #pragma endregion

        #pragma region Private variables
        int strideSize;

        Type _type;
        std::filesystem::path _path;

        ComPtr<ID3D11PixelShader> pixelShader;

        ComPtr<ID3D11VertexShader> vertexShader;


        std::vector<ConstantBufferData>   constantBuffers;
        std::vector<ConstantBufferVariable> constantVariables;
        std::unordered_map<std::string, int> textures;

        Renderer* _renderer;
        #pragma endregion

        #pragma region Private methods
        void loadVs(const std::filesystem::path& path, std::vector<Layout> layout);
        void loadPs(const std::filesystem::path& path);

        ComPtr<ID3D11InputLayout> createLayout(const ComPtr<ID3DBlob>& shaderBuffer, const ComPtr<ID3D11ShaderReflection>& reflection);
        ComPtr<ID3D11InputLayout> createOverridenLayout(const ComPtr<ID3DBlob>& shaderBuffer, std::vector<Layout> layout);

        void createVariableConstantBuffers(const ComPtr<ID3D11ShaderReflection>& reflection);
        void createTextureInfo(const ComPtr<ID3D11ShaderReflection>& reflection);

        static void compile(const std::filesystem::path& path, const std::string& entry, const std::string& target,
            ComPtr<ID3DBlob>& shaderBuffer);

        static ComPtr<ID3D11ShaderReflection> reflect(const ComPtr<ID3DBlob>& shaderBuffer);
        #pragma endregion
    };
}