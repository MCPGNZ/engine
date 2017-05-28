#pragma once
namespace pk
{
    class Texture;
    class SpriteSystem;

    class Sprite
    {
    public:
        Sprite(void) = default;

        Sprite(SpriteSystem& system);
        void texture(int texture);

    private:
        SpriteSystem& _system;
        int _texture;

        friend class SpriteSystem;

        friend class SpriteView;
        friend class SpriteSystemView;
    };
}
