#include "Vehicle.h"

#include "Sprite.h"
#include "SpriteSystem.h"

#include "Image.h"

#include "keyboard.h"

namespace pk
{
    Vehicle::Vehicle(SpriteSystem& spriteSystem) :
        _spriteSystem{spriteSystem}
    {
        _sprite = spriteSystem.add();

        auto& sprite = spriteSystem.sprite(_sprite);

        auto& transform = spriteSystem.transform(_sprite);
        transform.scale.x = 9.0f;
        transform.scale.y = 4.0f;
    }

    vec3f Vehicle::position()
    {
        return _spriteSystem.transform(_sprite).position;
    }

    void Vehicle::update(float deltaTime)
    {
        auto& transform = _spriteSystem.transform(_sprite);
        if(Keyboard::isKeyPressed(Keyboard::Key::W))
        {
            _velocity.x += 10.0f * deltaTime;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::S))
        {
            _velocity.x -= 10.0f * deltaTime;
        }

        if(Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            _angle += 60.0f * deltaTime;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            _angle -= 60.0f * deltaTime;
        }

        transform.position += transform.forward() * _velocity.x * deltaTime;
        transform.rotation.z = _angle;
    }
}