#pragma once

#include "pluto/physics_2d/components/collider_2d.h"
#include "pluto/physics_2d/physics_2d_body.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.h"

#include "pluto/scene/components/component.impl.hpp"

#include "pluto/math/vector2f.h"

namespace pluto
{
    class Collider2D::Impl : public Component::Impl
    {
        Physics2DShape* shape;
        std::shared_ptr<Physics2DBody> body;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject, Physics2DShape& shape,
             std::shared_ptr<Physics2DBody> body)
            : Component::Impl(guid, gameObject),
              shape(&shape),
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
    };
}
