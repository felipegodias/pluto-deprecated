#include "pluto/physics_2d/shapes/physics_2d_shape.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.impl.hpp"

namespace pluto
{
    Physics2DShape::~Physics2DShape() = default;

    Physics2DShape::Physics2DShape(Impl& impl)
        : impl(&impl)
    {
    }

    Physics2DShape::Physics2DShape(Physics2DShape&& other) noexcept
    {
        impl = other.impl;
        other.impl = nullptr;
    }

    Physics2DShape& Physics2DShape::operator=(Physics2DShape&& other) noexcept = default;

    float Physics2DShape::GetFriction() const
    {
        return impl->GetFriction();
    }

    void Physics2DShape::SetFriction(const float value)
    {
        impl->SetFriction(value);
    }

    float Physics2DShape::GetRestitution() const
    {
        return impl->GetRestitution();
    }

    void Physics2DShape::SetRestitution(const float value)
    {
        impl->SetRestitution(value);
    }

    bool Physics2DShape::IsTrigger() const
    {
        return impl->IsTrigger();
    }

    void Physics2DShape::SetTrigger(const bool value)
    {
        impl->SetTrigger(value);
    }
}
