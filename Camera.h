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

        vec3d direction() const;
        vec3d origin() const;
        vec3d up() const;

        void direction(const vec3d& v);
        void origin(const vec3d& v);

        Type type() const;

        void ortographic(const vec2i& dimentions, const double near = 0.0, const double far = 10.0);
        void perspective(const vec2i& dimentions, const degree fov = degree{45.0}, const double near = 0.1, const double far = 10.0);
        #pragma endregion

    private:
        #pragma region private variables
        Type _type;

        matrix4x4 _view;
        matrix4x4 _projection;

        vec3d _origin = vec3d::zero;
        vec3d _direction = vec3d::forward;
        vec3d _up = vec3d::up;

        vec3d _forward;
        vec3d _strafe;

        vec2i _dimentions;

        double _near;
        double _far;

        double _aspect;
        radian _fov;

        bool _dirty = false;
        #pragma endregion
    };
}
