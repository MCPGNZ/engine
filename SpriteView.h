#pragma once
namespace pk
{
    class SpriteSystem;

    class SpriteView
    {
    public:
        SpriteView() = default;
        SpriteView(int component, SpriteSystem& system);

        void render();

    private:
        int _component = -1;
        SpriteSystem* _system;
    };
}
