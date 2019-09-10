#pragma once

#include "pluto/physics_2d/shapes/physics_2d_shape.h"

#include <Box2D/Box2D.h>

namespace pluto
{
    class Physics2DShape::Impl
    {
    protected:
        b2Fixture* fixture;

    public:
        virtual ~Impl()
        {
            b2Body* body = fixture->GetBody();
            body->DestroyFixture(fixture);
        }

        explicit Impl(b2Fixture& fixture)
            : fixture(&fixture)
        {
        }

        Impl(const Impl& other) = delete;
        Impl(Impl&& other) noexcept = default;
        Impl& operator=(const Impl& rhs) = delete;
        Impl& operator=(Impl&& rhs) noexcept = default;
    };
}
