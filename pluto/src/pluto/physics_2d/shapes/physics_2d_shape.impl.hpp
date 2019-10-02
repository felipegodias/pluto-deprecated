#pragma once

#include "pluto/guid.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.h"

#include <Box2D/Box2D.h>

namespace pluto
{
    class Physics2DShape::Impl
    {
    protected:
        std::unique_ptr<Guid> colliderId;
        b2Fixture* fixture;

    public:
        virtual ~Impl()
        {
            b2Body* body = fixture->GetBody();
            body->DestroyFixture(fixture);
        }

        explicit Impl(std::unique_ptr<Guid> colliderId, b2Fixture& fixture)
            : colliderId(std::move(colliderId)),
              fixture(&fixture)
        {
        }

        Impl(const Impl& other) = delete;
        Impl(Impl&& other) noexcept = default;
        Impl& operator=(const Impl& rhs) = delete;
        Impl& operator=(Impl&& rhs) noexcept = default;

        float GetFriction() const
        {
            return fixture->GetFriction();
        }

        void SetFriction(const float value)
        {
            fixture->SetFriction(value);
        }

        float GetRestitution() const
        {
            return fixture->GetRestitution();
        }

        void SetRestitution(const float value)
        {
            fixture->SetRestitution(value);
        }

        bool IsTrigger() const
        {
            return fixture->IsSensor();
        }

        void SetTrigger(const bool value)
        {
            fixture->SetSensor(value);
        }
    };
}
