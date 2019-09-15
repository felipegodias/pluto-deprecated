#pragma once

#include "pluto/physics_2d/components/collider_2d.h"
#include "pluto/physics_2d/physics_2d_body.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.h"

#include "pluto/memory/resource.h"
#include "pluto/scene/components/transform.h"

#include "pluto/scene/components/component.impl.hpp"

#include "pluto/math/math.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/quaternion.h"

namespace pluto
{
    class Collider2D::Impl : public Component::Impl
    {
        Vector2F lastPosition;
        float lastAngle;

        Physics2DShape* shape;
        Resource<Transform> transform;
        std::shared_ptr<Physics2DBody> body;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject, Physics2DShape& shape,
             std::shared_ptr<Physics2DBody> body)
            : Component::Impl(guid, gameObject),
              lastPosition(body->GetPosition()),
              lastAngle(body->GetAngle()),
              shape(&shape),
              transform(gameObject->GetTransform()),
              body(std::move(body))

        {
        }

        Vector2F GetOffset() const
        {
            return shape->GetOffset();
        }

        void SetOffset(const Vector2F& value)
        {
            shape->SetOffset(value);
        }

        void OnEarlyFixedUpdate()
        {
            const Vector3F transformPosition = transform->GetPosition();
            const Vector2F position = {transformPosition.x, transformPosition.y};

            if (Vector2F::Distance(lastPosition, position) > Math::EPSILON)
            {
                body->SetPosition(position);
            }

            const Vector3F transformEulerAngles = transform->GetRotation().GetEulerAngles();
            const float angle = transformEulerAngles.z;
            if (abs(angle - lastAngle) > Math::EPSILON)
            {
                body->SetAngle(angle);
            }
        }

        void OnLateFixedUpdate()
        {
            lastPosition = body->GetPosition();
            lastAngle = body->GetAngle();
        }
    };
}
