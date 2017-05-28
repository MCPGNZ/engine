#include "Texture.h"

#include "image.h"
#include "Renderer.h"

namespace pk
{
    #pragma region Ctors
    Texture::Texture(const std::filesystem::path& path)
    {
        create(path);
    }
    Texture::Texture(const Image& image)
    {
        create(image);
    }
    bool Texture::operator==(const Texture & other) const
    {
        return _shaderView == other._shaderView;
    }
    #pragma endregion

    #pragma region Public methods
    void Texture::create(const std::filesystem::path& path)
    {
        create(Image{path});
    }
    void Texture::create(const Image& image)
    {
        (*this) = Renderer::createTexture(image);
    }
    void* Texture::id() const
    {
        return _shaderView.Get();
    }
    #pragma endregion
}