#include "pluto/physics_2d/shapes/physics_2d_box_shape.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.impl.hpp"
#include "pluto/physics_2d/physics_2d_body.h"

#include "pluto/debug/assert.h"
#include "pluto/math/vector2f.h"

#include <Box2D/Box2D.h>

namespace pluto
{
    class Physics2DBoxShape::Impl final : public Physics2DShape::Impl
    {
    public:
        ~Impl() override = default;

        explicit Impl(b2Fixture& fixture)
            : Physics2DShape::Impl(fixture)
        {
            ASSERT_THAT_IS_TRUE(fixture.GetType() == b2Shape::e_polygon);
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& rhs) = delete;

        Impl& operator=(Impl&& rhs) noexcept = default;

        Vector2F GetOffset() const
        {
            auto* shape = GetShape();
            return {shape->m_centroid.x, shape->m_centroid.y};
        }

        void SetOffset(const Vector2F& value)
        {
            auto* shape = GetShape();
            return shape->m_centroid.Set(value.x, value.y);
        }

        Vector2F GetSize() const
        {
            auto* shape = GetShape();
            const b2Vec2& min = shape->GetVertex(0);
            const b2Vec2& max = shape->GetVertex(2);
            return {max.x - min.x, max.y - min.y};
        }

        void SetSize(const Vector2F& value)
        {
            auto* shape = GetShape();
            shape->SetAsBox(value.x / 2, value.y / 2);
        }

    private:
        b2PolygonShape* GetShape() const
        {
            return dynamic_cast<b2PolygonShape*>(fixture->GetShape());
        }
    };

    Physics2DBoxShape::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Physics2DBoxShape> Physics2DBoxShape::Factory::Create(Physics2DBody& body,
                                                                          const Vector2F& offset,
                                                                          const Vector2F& size) const
    {
        auto* nativeBody = reinterpret_cast<b2Body*>(body.GetNativeBody());
        b2PolygonShape shape;
        shape.SetAsBox(size.x / 2, size.y / 2);
        shape.m_centroid.Set(offset.x, offset.y);

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1;
        fixtureDef.friction = 0.9f;
        fixtureDef.restitution = 0.1f;
        fixtureDef.shape = &shape;

        b2Fixture* fixture = nativeBody->CreateFixture(&fixtureDef);
        return std::make_unique<Physics2DBoxShape>(std::make_unique<Impl>(*fixture));
    }

    Physics2DBoxShape::~Physics2DBoxShape() = default;

    Physics2DBoxShape::Physics2DBoxShape(std::unique_ptr<Impl> impl)
        : Physics2DShape(*impl),
          impl(std::move(impl))
    {
    }

    Physics2DBoxShape::Physics2DBoxShape(Physics2DBoxShape&& other) noexcept = default;

    Physics2DBoxShape& Physics2DBoxShape::operator=(Physics2DBoxShape&& rhs) noexcept = default;

    Vector2F Physics2DBoxShape::GetOffset() const
    {
        return impl->GetOffset();
    }

    void Physics2DBoxShape::SetOffset(const Vector2F& value)
    {
        impl->SetOffset(value);
    }

    Vector2F Physics2DBoxShape::GetSize() const
    {
        return impl->GetSize();
    }

    void Physics2DBoxShape::SetSize(const Vector2F& value)
    {
        impl->SetSize(value);
    }
}
