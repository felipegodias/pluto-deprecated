#include "pluto/physics_2d/physics_2d_body.h"
#include "pluto/physics_2d/physics_2d_manager.h"
#include "pluto/physics_2d/shapes/physics_2d_shape.h"
#include "pluto/physics_2d/shapes/physics_2d_circle_shape.h"

#include "pluto/service/service_collection.h"
#include "pluto/math/math.h"
#include "pluto/math/vector2f.h"

#include <Box2D/Box2D.h>

#include <vector>

namespace pluto
{
    class Physics2DBody::Impl
    {
        b2Body* body;
        std::vector<std::unique_ptr<Physics2DShape>> shapes;
        Physics2DCircleShape::Factory* circleShapeFactory;
        Physics2DBody* instance;

    public:
        ~Impl()
        {
            shapes.clear();
            b2World* world = body->GetWorld();
            world->DestroyBody(body);
            body = nullptr;
        }

        Impl(b2Body& body, Physics2DCircleShape::Factory& circleShapeFactory)
            : body(&body),
              circleShapeFactory(&circleShapeFactory),
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

        Physics2DCircleShape& CreateCircleShape(const Vector2F& offset, const float radius)
        {
            std::unique_ptr<Physics2DCircleShape> shape = circleShapeFactory->Create(*instance, offset, radius);
            Physics2DCircleShape* ptr = shape.get();
            shapes.push_back(std::move(shape));
            return *ptr;
        }

        void DestroyShape(const Physics2DShape& shape)
        {
            auto it = shapes.begin();
            while (it != shapes.end())
            {
                if ((*it).get() == &shape)
                {
                    shapes.erase(it);
                    break;
                }
                ++it;
            }
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

    std::unique_ptr<Physics2DBody> Physics2DBody::Factory::Create(const Vector2F& position, const float angle) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& physics2DManager = serviceCollection.GetService<Physics2DManager>();
        auto& circleShapeFactory = serviceCollection.GetFactory<Physics2DCircleShape>();

        b2BodyDef bodyDef;
        bodyDef.position = {position.x, position.y};
        bodyDef.angle = angle;

        auto* world = reinterpret_cast<b2World*>(physics2DManager.GetWorld());
        b2Body* body = world->CreateBody(&bodyDef);

        auto physics2DBody = std::make_unique<Physics2DBody>(std::make_unique<Impl>(*body, circleShapeFactory));
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

    Physics2DBody::Type Physics2DBody::GetType(Type type) const
    {
        return impl->GetType();
    }

    void Physics2DBody::SetType(const Type value)
    {
        impl->SetType(value);
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

    Physics2DCircleShape& Physics2DBody::CreateCircleShape(const Vector2F& offset, const float radius)
    {
        return impl->CreateCircleShape(offset, radius);
    }

    void Physics2DBody::DestroyShape(const Physics2DShape& shape)
    {
        impl->DestroyShape(shape);
    }

    void* Physics2DBody::GetNativeBody() const
    {
        return impl->GetNativeBody();
    }
}
