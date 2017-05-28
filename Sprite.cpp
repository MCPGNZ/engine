#include "Sprite.h"
#include "SpriteSystem.h"

#include "image.h"

namespace pk
{
    Sprite::Sprite(SpriteSystem& spriteSystem) :
        _system{spriteSystem}
    {
        static Image defaultImage{vec2i{32}, Color::default};
        static Texture defaultTexture{defaultImage};

        _texture = -1;
    }

    void Sprite::texture(int texture)
    {
        _texture = texture;
    }
}