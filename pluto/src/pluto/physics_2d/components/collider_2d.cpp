#include "pluto/physics_2d/components/collider_2d.h"
#include "pluto/physics_2d/components/collider_2d.impl.hpp"

namespace pluto
{
    Collider2D::~Collider2D() = default;

    Collider2D::Collider2D(Impl& impl)
        : Component(impl),
          impl(&impl)
    {
    }

    Collider2D::Collider2D(Collider2D&& other) noexcept
        : Component(std::move(other))
    {
        impl = other.impl;
        other.impl = nullptr;
    }

    Collider2D& Collider2D::operator=(Collider2D&& rhs) noexcept = default;

    float Collider2D::GetFriction() const
    {
        return impl->GetFriction();
    }

    void Collider2D::SetFriction(const float value)
    {
        impl->SetFriction(value);
    }

    float Collider2D::GetRestitution() const
    {
        return impl->GetFriction();
    }

    void Collider2D::SetRestitution(const float value)
    {
        impl->SetRestitution(value);
    }

    bool Collider2D::IsTrigger() const
    {
        return impl->IsTrigger();
    }

    void Collider2D::SetTrigger(const bool value)
    {
        impl->SetTrigger(value);
    }

    Vector2F Collider2D::GetOffset() const
    {
        return impl->GetOffset();
    }

    void Collider2D::SetOffset(const Vector2F& value)
    {
        impl->SetOffset(value);
    }

    void Collider2D::OnEarlyFixedUpdate()
    {
        impl->OnEarlyFixedUpdate();
    }

    void Collider2D::OnLateFixedUpdate()
    {
        impl->OnLateFixedUpdate();
    }
}
