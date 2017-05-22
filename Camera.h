#pragma once
#include "matrix4x4.h"
#include "vec3.h"
#include "degree.h"
#include "radian.h"

namespace pk
{
    class Camera
    {
    public:
        #pragma region types
        enum struct Type
        {
            Orthogonal,
            Perspective
        };
        #pragma endregion

        #pragma region ctors
        Camera(void);
        #pragma endregion

        #pragma region public methods
        matrix4x4 view();
        matrix4x4 projection() const;

        vec3f direction() const;
        vec3f origin() const;
        vec3f up() const;

        void direction(const vec3f& v);
        void origin(const vec3f& v);

        Type type() const;

        void ortographic(const vec2i& dimentions, const float near = 0.0, const float far = 10.0);
        void perspective(const vec2i& dimentions, const degree fov = degree{45.0}, const float near = 0.1, const float far = 10.0);
        #pragma endregion

    private:
        #pragma region private variables
        Type _type;

        matrix4x4 _view;
        matrix4x4 _projection;

        vec3f _origin = vec3f::zero;
        vec3f _direction = vec3f::forward;
        vec3f _up = vec3f::up;

        vec3f _forward;
        vec3f _strafe;

        vec2i _dimentions;

        float _near;
        float _far;

        float _aspect;
        radian _fov;

        bool _dirty = false;
        #pragma endregion
    };
}
