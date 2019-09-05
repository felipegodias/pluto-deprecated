#include "pluto/physics_2d/shapes/physics_2d_shape.h"

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
}
