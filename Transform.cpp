#include "Transform.h"
#include "radian.h"
#include "degree.h"

#include "json.hpp"
#include "json_bindings.h"

namespace pk
{
    matrix4x4 Transform::worldMatrix() const
    {
        return math::srt(position, vec3<radian>{pk::degree{rotation.x}, pk::degree{rotation.y}, pk::degree{rotation.z}}, scale);
    }
    matrix4x4 Transform::rotationMatrix() const
    {
        return math::rotation(vec3<radian>{pk::degree{rotation.x}, pk::degree{rotation.y}, pk::degree{rotation.z}});
    }

    vec3f Transform::forward() const
    {
        auto local = vec3f{1.0f, 0.0f, 0.0f};
        auto matrix = rotationMatrix();

        return local * matrix;
    }

    #pragma region serialization
    void to_json(json& j, const Transform & transform)
    {
        j = json{{"transform", {
                    {"position", transform.position},
                    {"rotation", transform.rotation},
                    {"scale", transform.scale}
                }}
        };
    }

    void from_json(const json& j, Transform & transform)
    {
        const auto& data = j["transform"];
        transform.position = data.at("position").get<vec3f>();
        transform.rotation = data.at("rotation").get<vec3f>();
        transform.scale = data.at("scale").get<vec3f>();
    }
    #pragma endregion
}