#include "Transform.h"
#include "radian.h"
#include "degree.h"

namespace pk
{
    matrix4x4 Transform::world()
    {
        return math::srt(position, vec3<radian>{pk::degree{rotation.x}, pk::degree{rotation.y}, pk::degree{rotation.z}}, scale);
    }
}