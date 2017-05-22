#include "Shader.h"

#include <d3d10.h>
#include <d3dcompiler.h>

#include "File.h"
#include "logger.h"
#include "Renderer.h"
#include "Texture.h"

#include "GPUDesc.h"
#include "UtilityDX11.h"


namespace pk
{
    #pragma region Ctors
    Shader::Shader(const std::filesystem::path& path, std::vector<Layout> layout /* = {} */)
    {
        load(path, layout);
    }
    #pragma endregion

    #pragma region Public methods

    int Shader::stride() const
    {
        return strideSize;
    }

    void Shader::load(const std::filesystem::path& path, std::vector<Layout> layout /* = {} */)
    {
        _path = path;
        if(!std::filesystem::exists(path))
        {
            logger::error("Shader", "load", "file does not exists");
            return;
        }

        const auto& extension = path.extension();
        if(extension == ".ps")
        {
            _type = Type::PixelShader;
            loadPs(path);
            return;
        }

        if(extension == ".vs")
        {
            _type = Type::VertexShader;
            loadVs(path, layout);
            return;
        }
        if(extension == ".hlsl")
        {
            if(isPS(path))
            {
                _type = Type::PixelShader;
                loadPs(path);
                return;
            }
            if(isVS(path))
            {
                _type = Type::VertexShader;
                loadVs(path, layout);
                return;
            }
        }

        logger::error("Shader", "load", "extension not recognized");
    }
    void Shader::reload()
    {
        load(_path);
        _renderer->set(*this);
    }

    bool Shader::exists(const std::string& name) const
    {
        for(const auto& variable : constantVariables)
        {
            if(variable.name == name)
            {
                return true;
            }
        }
        return false;
    }

    Shader::Type Shader::type() const
    {
        return _type;
    }

    void Shader::set(const std::string& name, void* data)
    {
        if(textures.find(name) != textures.end())
        {
            auto& texture = *reinterpret_cast<pk::Texture*>(data);
            switch(_type)
            {
                case Type::PixelShader:
                {
                    Renderer::_context()->PSSetShaderResources(textures[name], 1, texture._shaderResourceViewAddres());
                    break;
                }

                case Type::VertexShader:
                {
                    Renderer::_context()->PSSetShaderResources(textures[name], 1, texture._shaderResourceViewAddres());
                    break;
                }
            }
            return;
        }

        for(const auto& variable : constantVariables)
        {
            if(variable.name == name)
            {
                auto& bufferData = constantBuffers[variable.bufferIndex];
                memcpy(bufferData.bytes.data() + variable.offset, data, variable.size);

                bufferData.buffer.send(bufferData.bytes.data());
                return;
            }
        }

        logger::warning("Shader", "set", ("shader variable named: " + name + " not found").c_str());
    }

    void* Shader::get(const std::string & name)
    {
        for(const auto& variable : constantVariables)
        {
            if(variable.name == name)
            {
                auto& bufferData = constantBuffers[variable.bufferIndex];
                return bufferData.bytes.data() + variable.offset;
            }
        }

        logger::warning("Shader", "set", ("shader variable named: " + name + " not found").c_str());

        return nullptr;
    }
    #pragma endregion

    #pragma region Private methods

    void Shader::loadPs(const std::filesystem::path& path)
    {
        ComPtr<ID3DBlob> shaderBuffer = nullptr;
        compile(path, "PS", "ps_4_0", shaderBuffer);

        if(shaderBuffer == nullptr) return;

        auto hr = Renderer::_device()->CreatePixelShader(
            shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &pixelShader);

        logger::assert_hr(hr, "Shader", "loadPs", "CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &pixelShader);");

        const auto reflection = reflect(shaderBuffer);
        createVariableConstantBuffers(reflection);
        createTextureInfo(reflection);
    }
    void Shader::loadVs(const std::filesystem::path& path, std::vector<Layout> layout)
    {
        ComPtr<ID3DBlob> shaderBuffer = nullptr;
        compile(path, "VS", "vs_4_0", shaderBuffer);

        if(shaderBuffer == nullptr) return;

        auto hr = Renderer::_device()->CreateVertexShader(
            shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &vertexShader);

        const auto reflection = reflect(shaderBuffer);

        if(layout.size() == 0)
        {
            _layout = createLayout(shaderBuffer, reflection);
        }
        else
        {
            _layout = createOverridenLayout(shaderBuffer, layout);
        }

        createVariableConstantBuffers(reflection);
        createTextureInfo(reflection);
    }

    bool Shader::isPS(const std::filesystem::path& path)
    {
        const auto code = File::readAll(path);

        ComPtr<ID3DBlob> shaderBuffer = nullptr;
        ComPtr<ID3DBlob> errorMessage = nullptr;

        auto hr = D3DCompile(
            code.data(),
            code.size(),
            nullptr,
            nullptr,
            nullptr,
            "PS",
            "ps_4_0",
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &shaderBuffer,
            &errorMessage
        );

        return !FAILED(hr);
    }
    bool Shader::isVS(const std::filesystem::path& path)
    {
        const auto code = File::readAll(path);

        ComPtr<ID3DBlob> shaderBuffer = nullptr;
        ComPtr<ID3DBlob> errorMessage = nullptr;

        auto hr = D3DCompile(
            code.data(),
            code.size(),
            nullptr,
            nullptr,
            nullptr,
            "VS",
            "vs_4_0",
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &shaderBuffer,
            &errorMessage
        );

        return !FAILED(hr);
    }

    void Shader::compile(const std::filesystem::path& path, const std::string& entry, const std::string& target,
        ComPtr<ID3DBlob>& shaderBuffer)
    {
        const auto code = File::readAll(path);

        ComPtr<ID3DBlob> errorMessage = nullptr;

        auto hr = D3DCompile(
            code.data(),
            code.size(),
            nullptr,
            nullptr,
            nullptr,
            entry.c_str(),
            target.c_str(),
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &shaderBuffer,
            &errorMessage
        );

        logger::assert_hr(hr, "Shader", "compile", errorMessage == nullptr ? "" : reinterpret_cast<char*>(errorMessage->GetBufferPointer()));
    }

    ComPtr<ID3D11ShaderReflection> Shader::reflect(const ComPtr<ID3DBlob>& shaderBuffer)
    {
        ComPtr<ID3D11ShaderReflection> result;

        auto hr = D3DReflect(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(),
            IID_ID3D11ShaderReflection, (void**) &result);

        logger::assert_hr(hr, "Shader", "reflect", "D3DReflect");

        return result;
    }

    ComPtr<ID3D11InputLayout> Shader::createLayout(const ComPtr<ID3DBlob>& shaderBuffer, const ComPtr<ID3D11ShaderReflection>& reflection)
    {
        ComPtr<ID3D11InputLayout> result;

        strideSize = 0;

        D3D11_SHADER_DESC shaderDesc;
        reflection->GetDesc(&shaderDesc);

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
        for(int i = 0; i < static_cast<int>(shaderDesc.InputParameters); ++i)
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            reflection->GetInputParameterDesc(i, &paramDesc);

            int components = -1;
            if(paramDesc.Mask <= 15) components = 4;
            if(paramDesc.Mask <= 7)  components = 3;
            if(paramDesc.Mask <= 3)  components = 2;
            if(paramDesc.Mask == 1)  components = 1;

            strideSize += components * 4;

            D3D11_INPUT_ELEMENT_DESC elementDesc;
            elementDesc.Format = dx::ComponentToFormat(paramDesc.ComponentType, components, paramDesc.SemanticName);
            elementDesc.SemanticName = paramDesc.SemanticName;
            elementDesc.SemanticIndex = paramDesc.SemanticIndex;
            elementDesc.InputSlot = 0;
            elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            elementDesc.InstanceDataStepRate = 0;

            inputDesc.push_back(elementDesc);
        }

        auto hr = Renderer::_device()->CreateInputLayout(inputDesc.data(), inputDesc.size(), shaderBuffer->GetBufferPointer(),
            shaderBuffer->GetBufferSize(), result.GetAddressOf());
        logger::assert_hr(hr, "Shader", "createLayout", "CreateInputLayout");
        return result;
    }
    ComPtr<ID3D11InputLayout> Shader::createOverridenLayout(const ComPtr<ID3DBlob>& shaderBuffer, std::vector<Layout> input)
    {
        ComPtr<ID3D11InputLayout> result;

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
        for(auto& entry : input)
        {
            D3D11_INPUT_ELEMENT_DESC desc;
            desc.Format = dx::Convert(entry.format);
            desc.SemanticName = dx::GetSemanticName(entry.layout);
            desc.SemanticIndex = entry.index;
            desc.InputSlot = 0;
            desc.AlignedByteOffset = entry.offset == -1 ? D3D11_APPEND_ALIGNED_ELEMENT : entry.offset;
            desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            desc.InstanceDataStepRate = 0;

            inputDesc.push_back(desc);
        }

        _layout = nullptr;

        auto hr = Renderer::_device()->CreateInputLayout(inputDesc.data(), inputDesc.size(), shaderBuffer->GetBufferPointer(),
            shaderBuffer->GetBufferSize(), result.GetAddressOf());
        logger::assert_hr(hr, "Shader", "createOverridenLayout", "CreateInputLayout");
        return result;
    }

    void Shader::createVariableConstantBuffers(const ComPtr<ID3D11ShaderReflection>& reflection)
    {
        constantBuffers.clear();
        constantVariables.clear();

        D3D11_SHADER_DESC shaderDescription;
        reflection->GetDesc(&shaderDescription);

        const int cbSize = static_cast<const int>(shaderDescription.ConstantBuffers);
        for(int cb = 0; cb < cbSize; ++cb)
        {
            const auto& constantBufferReflection = reflection->GetConstantBufferByIndex(cb);

            D3D11_SHADER_BUFFER_DESC bufferDescription;
            constantBufferReflection->GetDesc(&bufferDescription);

            ConstantBufferData data;
            data.bytes.resize(bufferDescription.Size);
            data.buffer = Buffer::constant(data.bytes.data(), bufferDescription.Size);
            data.index = cb;

            const auto bufferHandle = constantBuffers.size();
            constantBuffers.emplace_back(data);

            const int varSize = static_cast<int>(bufferDescription.Variables);
            for(int var = 0; var < varSize; ++var)
            {
                const auto& variableReflection = constantBufferReflection->GetVariableByIndex(var);

                D3D11_SHADER_VARIABLE_DESC variableDescription;
                variableReflection->GetDesc(&variableDescription);

                ConstantBufferVariable variable;
                variable.bufferIndex = bufferHandle;
                variable.name = variableDescription.Name;
                variable.size = variableDescription.Size;
                variable.offset = variableDescription.StartOffset;

                constantVariables.emplace_back(variable);
            }
        }
    }
    void Shader::createTextureInfo(const ComPtr<ID3D11ShaderReflection>& reflection)
    {
        textures.clear();

        D3D11_SHADER_INPUT_BIND_DESC desc;
        UINT i = 0;
        do
        {
            ZeroMemory(&desc, sizeof(D3D11_SHADER_INPUT_BIND_DESC));
            reflection->GetResourceBindingDesc(i, &desc);

            if(desc.Type == D3D_SIT_TEXTURE)
            {
                textures[desc.Name] = desc.BindPoint;
            }
            ++i;
        }
        while(desc.Name != 0);
    }
    #pragma endregion
}