#include "Texture.h"

#include "image.h"
#include "Renderer.h"

namespace pk
{
    #pragma region Ctors
    Texture::Texture(const std::string& path)
    {
        create(path);
    }
    Texture::Texture(const Image& image)
    {
        create(image);
    }
    #pragma endregion

    #pragma region Public methods
    void Texture::create(const std::string& path)
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