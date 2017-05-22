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

    vec3d Camera::direction() const
    {
        return _direction;
    }
    vec3d Camera::origin() const
    {
        return _origin;
    }
    vec3d Camera::up() const
    {
        return _up;
    }

    void Camera::direction(const vec3d& dir)
    {
        _up = math::cross(dir, vec3d::right);

        _forward = math::normalize(dir);
        _strafe = math::normalize(math::cross(_up, dir));

        _direction = _origin + _forward;

        _dirty = true;
    }
    void Camera::origin(const vec3d& position)
    {
        _origin = position;
        _dirty = true;
    }

    Camera::Type Camera::type() const
    {
        return _type;
    }

    void Camera::ortographic(const vec2i& dimentions, const double near /* = 0.0 */, const double far /* = 10.0 */)
    {
        _type = Type::Orthogonal;

        _dimentions = dimentions;

        _near = near;
        _far = far;

        _projection = math::orthogonal(static_cast<vec2d>(dimentions), vec2d{_near, _far});
        _view = math::look(_origin, _direction, _up);
    }

    void Camera::perspective(const vec2i& dimentions, const degree fov /*= 45.0*/, const double near /*= 0.1*/, const double far /*= 10.0*/)
    {
        _type = Type::Perspective;

        _dimentions = dimentions;
        _fov = fov;

        _near = near;
        _far = far;

        _aspect = static_cast<double>(_dimentions.width) / static_cast<double>(_dimentions.height);

        _projection = math::perspective(_fov, _aspect, vec2d{_near, _far});
        _view = math::look(_origin, _direction, _up);
    }
    #pragma endregion
}