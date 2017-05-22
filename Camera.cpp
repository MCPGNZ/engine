#include "Camera.h"

namespace pk
{
    #pragma region ctors
    Camera::Camera(void)
    {}
    #pragma endregion

    #pragma region public methods
    matrix4x4 Camera::view()
    {
        if(_dirty)
        {
            _view = math::look(_origin, _direction, _up);
            _dirty = false;
        }

        return _view;
    }
    matrix4x4 Camera::projection() const
    {
        return _projection;
    }

    vec3f Camera::direction() const
    {
        return _direction;
    }
    vec3f Camera::origin() const
    {
        return _origin;
    }
    vec3f Camera::up() const
    {
        return _up;
    }

    void Camera::direction(const vec3f& dir)
    {
        _up = math::cross(dir, vec3f::right);

        _forward = math::normalize(dir);
        _strafe = math::normalize(math::cross(_up, dir));

        _direction = _origin + _forward;

        _dirty = true;
    }
    void Camera::origin(const vec3f& position)
    {
        _origin = position;
        _dirty = true;
    }

    Camera::Type Camera::type() const
    {
        return _type;
    }

    void Camera::ortographic(const vec2i& dimentions, const float near /* = 0.0 */, const float far /* = 10.0 */)
    {
        _type = Type::Orthogonal;

        _dimentions = dimentions;

        _near = near;
        _far = far;

        _projection = math::orthogonal(static_cast<vec2f>(dimentions), vec2f{_near, _far});
        _view = math::look(_origin, _direction, _up);
    }

    void Camera::perspective(const vec2i& dimentions, const degree fov /*= 45.0*/, const float near /*= 0.1*/, const float far /*= 10.0*/)
    {
        _type = Type::Perspective;

        _dimentions = dimentions;
        _fov = fov;

        _near = near;
        _far = far;

        _aspect = static_cast<float>(_dimentions.width) / static_cast<float>(_dimentions.height);

        _projection = math::perspective(_fov, _aspect, vec2f{_near, _far});
        _view = math::look(_origin, _direction, _up);
    }
    #pragma endregion
}