#include "pluto/physics_2d/shapes/physics_2d_circle_shape.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.impl.hpp"
#include "pluto/physics_2d/physics_2d_body.h"

#include "pluto/debug/assert.h"
#include "pluto/math/vector2f.h"

#include <Box2D/Box2D.h>

namespace pluto
{
    class Physics2DCircleShape::Impl final : public Physics2DShape::Impl
    {
    public:
        ~Impl() override = default;

        explicit Impl(b2Fixture& fixture)
            : Physics2DShape::Impl(fixture)
        {
            ASSERT_THAT_IS_TRUE(fixture.GetType() == b2Shape::e_circle);
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& rhs) = delete;

        Impl& operator=(Impl&& rhs) noexcept = default;

        Vector2F GetOffset() const
        {
            auto* shape = GetShape();
            return {shape->m_p.x, shape->m_p.y};
        }

        void SetOffset(const Vector2F& value)
        {
            auto* shape = GetShape();
            return shape->m_p.Set(value.x, value.y);
        }

        float GetRadius() const
        {
            auto* shape = GetShape();
            return shape->m_radius;
        }

        void SetRadius(const float value)
        {
            auto* shape = GetShape();
            shape->m_radius = value;
        }

    private:
        b2CircleShape* GetShape() const
        {
            return dynamic_cast<b2CircleShape*>(fixture->GetShape());
        }
    };

    Physics2DCircleShape::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Physics2DCircleShape> Physics2DCircleShape::Factory::Create(Physics2DBody& body,
                                                                                const Vector2F& offset,
                                                                                const float radius) const
    {
        auto* nativeBody = reinterpret_cast<b2Body*>(body.GetNativeBody());
        b2CircleShape shape;
        shape.m_p.Set(offset.x, offset.y);
        shape.m_radius = radius;
        b2Fixture* fixture = nativeBody->CreateFixture(&shape, 1);
        return std::make_unique<Physics2DCircleShape>(std::make_unique<Impl>(*fixture));
    }

    Physics2DCircleShape::~Physics2DCircleShape() = default;

    Physics2DCircleShape::Physics2DCircleShape(std::unique_ptr<Impl> impl)
        : Physics2DShape(*impl),
          impl(std::move(impl))
    {
    }

    Physics2DCircleShape::Physics2DCircleShape(Physics2DCircleShape&& other) noexcept = default;

    Physics2DCircleShape& Physics2DCircleShape::operator=(Physics2DCircleShape&& other) noexcept = default;

    Vector2F Physics2DCircleShape::GetOffset() const
    {
        return impl->GetOffset();
    }

    void Physics2DCircleShape::SetOffset(const Vector2F& value)
    {
        impl->SetOffset(value);
    }

    float Physics2DCircleShape::GetRadius() const
    {
        return impl->GetRadius();
    }

    void Physics2DCircleShape::SetRadius(const float value)
    {
        impl->SetRadius(value);
    }
}
