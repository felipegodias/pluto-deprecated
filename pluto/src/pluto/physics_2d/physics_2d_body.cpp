#include "pluto/physics_2d/physics_2d_body.h"
#include "pluto/physics_2d/physics_2d_manager.h"
#include "pluto/physics_2d/shapes/physics_2d_circle_shape.h"
#include "pluto/physics_2d/shapes/physics_2d_box_shape.h"

#include "pluto/memory/resource.h"
#include "pluto/scene/game_object.h"

#include "pluto/service/service_collection.h"
#include "pluto/math/math.h"
#include "pluto/math/vector2f.h"

#include <Box2D/Box2D.h>

namespace pluto
{
    class Physics2DBody::Impl
    {
        std::unique_ptr<Guid> gameObjectId;
        float density;

        b2Body* body;
        Physics2DCircleShape::Factory* circleShapeFactory;
        Physics2DBoxShape::Factory* boxShapeFactory;
        Physics2DBody* instance;

    public:
        ~Impl()
        {
            b2World* world = body->GetWorld();
            world->DestroyBody(body);
            body = nullptr;
        }

        Impl(std::unique_ptr<Guid> gameObjectId, b2Body& body, Physics2DCircleShape::Factory& circleShapeFactory,
             Physics2DBoxShape::Factory& boxShapeFactory)
            : gameObjectId(std::move(gameObjectId)),
              density(1.0f),
              body(&body),
              circleShapeFactory(&circleShapeFactory),
              boxShapeFactory(&boxShapeFactory),
              instance(nullptr)
        {
        }

        Impl(const Impl& other) = delete;
        Impl(Impl&& other) noexcept = default;
        Impl& operator=(const Impl& other) = delete;
        Impl& operator=(Impl&& other) noexcept = default;

        void SetInstance(Physics2DBody& value)
        {
            this->instance = &value;
        }

        float GetDensity() const
        {
            return density;
        }

        void SetDensity(const float value)
        {
            density = value;
            for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
            {
                fixture->SetDensity(density);
            }
        }

        Type GetType() const
        {
            switch (body->GetType())
            {
            case b2_staticBody:
                return Type::Static;
            case b2_kinematicBody:
                return Type::Kinematic;
            case b2_dynamicBody:
                return Type::Dynamic;
            default:
                return Type::Default;
            }
        }

        void SetType(const Type value)
        {
            switch (value)
            {
            case Type::Static:
                body->SetType(b2_staticBody);
                break;
            case Type::Kinematic:
                body->SetType(b2_kinematicBody);
                break;
            case Type::Dynamic:
                body->SetType(b2_dynamicBody);
                break;
            default:
                body->SetType(b2_staticBody);
            }
        }

        float GetGravityScale() const
        {
            return body->GetGravityScale();
        }

        void SetGravityScale(const float value)
        {
            body->SetGravityScale(value);
        }

        Vector2F GetPosition() const
        {
            const b2Vec2 position = body->GetPosition();
            return {position.x, position.y};
        }

        void SetPosition(const Vector2F& value)
        {
            body->SetTransform({value.x, value.y}, body->GetAngle());
        }

        float GetAngle() const
        {
            return Math::Degrees(body->GetAngle());
        }

        void SetAngle(const float value)
        {
            body->SetTransform(body->GetPosition(), Math::Radians(value));
        }

        Vector2F GetVelocity() const
        {
            const b2Vec2 velocity = body->GetLinearVelocity();
            return {velocity.x, velocity.y};
        }

        void SetVelocity(const Vector2F& value)
        {
            body->SetLinearVelocity({value.x, value.y});
        }

        float GetAngularVelocity() const
        {
            return body->GetAngularVelocity();
        }

        void SetAngularVelocity(const float value)
        {
            body->SetAngularVelocity(value);
        }

        void AddForce(const Vector2F& force)
        {
            body->ApplyForceToCenter({force.x, force.y}, true);
        }

        void AddForce(const Vector2F& force, const Vector2F& point)
        {
            body->ApplyForce({force.x, force.y}, {point.x, point.y}, true);
        }

        void AddTorque(const float torque)
        {
            body->ApplyTorque(torque, true);
        }

        std::unique_ptr<Physics2DCircleShape> CreateCircleShape(const Guid& colliderId, const Vector2F& offset,
                                                                const float radius)
        {
            return circleShapeFactory->Create(*instance, colliderId, offset, radius);
        }

        std::unique_ptr<Physics2DBoxShape> CreateBoxShape(const Guid& colliderId, const Vector2F& offset,
                                                          const Vector2F& size)
        {
            return boxShapeFactory->Create(*instance, colliderId, offset, size);
        }

        void* GetNativeBody() const
        {
            return body;
        }
    };

    Physics2DBody::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Physics2DBody> Physics2DBody::Factory::Create(Resource<GameObject> gameObject,
                                                                  const Vector2F& position, const float angle) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& physics2DManager = serviceCollection.GetService<Physics2DManager>();
        auto& circleShapeFactory = serviceCollection.GetFactory<Physics2DCircleShape>();
        auto& boxShapeFactory = serviceCollection.GetFactory<Physics2DBoxShape>();

        std::unique_ptr<Guid> gameObjectId = std::make_unique<Guid>(gameObject.GetObjectId());

        b2BodyDef bodyDef;
        bodyDef.position = {position.x, position.y};
        bodyDef.angle = angle;
        bodyDef.userData = gameObjectId.get();

        auto* world = reinterpret_cast<b2World*>(physics2DManager.GetWorld());
        b2Body* body = world->CreateBody(&bodyDef);

        auto physics2DBody = std::make_unique<Physics2DBody>(
            std::make_unique<Impl>(std::move(gameObjectId), *body, circleShapeFactory, boxShapeFactory));
        physics2DBody->impl->SetInstance(*physics2DBody);
        return physics2DBody;
    }

    Physics2DBody::~Physics2DBody() = default;

    Physics2DBody::Physics2DBody(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    Physics2DBody::Physics2DBody(Physics2DBody&& other) noexcept = default;

    Physics2DBody& Physics2DBody::operator=(Physics2DBody&& rhs) noexcept = default;

    float Physics2DBody::GetDensity() const
    {
        return impl->GetDensity();
    }

    void Physics2DBody::SetDensity(const float value)
    {
        return impl->SetDensity(value);
    }

    Physics2DBody::Type Physics2DBody::GetType(Type type) const
    {
        return impl->GetType();
    }

    void Physics2DBody::SetType(const Type value)
    {
        impl->SetType(value);
    }

    float Physics2DBody::GetGravityScale() const
    {
        return impl->GetGravityScale();
    }

    void Physics2DBody::SetGravityScale(const float value)
    {
        impl->SetGravityScale(value);
    }

    Vector2F Physics2DBody::GetPosition() const
    {
        return impl->GetPosition();
    }

    void Physics2DBody::SetPosition(const Vector2F& value)
    {
        impl->SetPosition(value);
    }

    float Physics2DBody::GetAngle() const
    {
        return impl->GetAngle();
    }

    void Physics2DBody::SetAngle(const float value)
    {
        impl->SetAngle(value);
    }

    Vector2F Physics2DBody::GetVelocity() const
    {
        return impl->GetVelocity();
    }

    void Physics2DBody::SetVelocity(const Vector2F& value)
    {
        impl->SetVelocity(value);
    }

    float Physics2DBody::GetAngularVelocity() const
    {
        return impl->GetAngularVelocity();
    }

    void Physics2DBody::SetAngularVelocity(const float value)
    {
        impl->SetAngularVelocity(value);
    }

    void Physics2DBody::AddForce(const Vector2F& force)
    {
        impl->AddForce(force);
    }

    void Physics2DBody::AddForce(const Vector2F& force, const Vector2F& point)
    {
        impl->AddForce(force, point);
    }

    void Physics2DBody::AddTorque(const float torque)
    {
        impl->AddTorque(torque);
    }

    std::unique_ptr<Physics2DCircleShape> Physics2DBody::CreateCircleShape(
        const Guid& colliderId, const Vector2F& offset, const float radius)
    {
        return impl->CreateCircleShape(colliderId, offset, radius);
    }

    std::unique_ptr<Physics2DBoxShape> Physics2DBody::CreateBoxShape(const Guid& colliderId, const Vector2F& offset,
                                                                     const Vector2F& size)
    {
        return impl->CreateBoxShape(colliderId, offset, size);
    }

    void* Physics2DBody::GetNativeBody() const
    {
        return impl->GetNativeBody();
    }
}
