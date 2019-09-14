#include "pluto/physics_2d/components/rigidbody_2d.h"
#include "pluto/physics_2d/physics_2d_body.h"
#include "pluto/physics_2d/physics_2d_manager.h"

#include "pluto/service/service_collection.h"

#include "pluto/scene/components/transform.h"
#include "pluto/scene/components/component.impl.hpp"
#include "pluto/memory/resource.h"

#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/quaternion.h"

#include "pluto/guid.h"

namespace pluto
{
    class Rigidbody2D::Impl : public Component::Impl
    {
        Resource<Transform> transform;
        std::shared_ptr<Physics2DBody> body;

    public:
        explicit Impl(const Guid& guid, const Resource<GameObject>& gameObject,
                      std::shared_ptr<Physics2DBody> body)
            : Component::Impl(guid, gameObject),
              transform(gameObject->GetTransform()),
              body(std::move(body))
        {
        }

        Vector2F GetVelocity() const
        {
            return body->GetVelocity();
        }

        void SetVelocity(const Vector2F& value)
        {
            body->SetVelocity(value);
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
            body->AddForce(force);
        }

        void AddForce(const Vector2F& force, const Vector2F& point)
        {
            body->AddForce(force, point);
        }

        void AddTorque(const float torque)
        {
            body->AddTorque(torque);
        }

        void OnUpdate()
        {
            const Vector2F bodyPosition = body->GetPosition();
            const float bodyAngle = body->GetAngle();

            Vector3F transformPosition = transform->GetPosition();
            transformPosition.x = bodyPosition.x;
            transformPosition.y = bodyPosition.y;
            transform->SetPosition(transformPosition);

            Vector3F transformEulerAngles = transform->GetRotation().GetEulerAngles();
            transformEulerAngles.z = bodyAngle;
            transform->SetRotation(Quaternion::Euler(transformEulerAngles));
        }
    };

    Rigidbody2D::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> Rigidbody2D::Factory::Create(const Resource<GameObject>& gameObject) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& physics2DManager = serviceCollection.GetService<Physics2DManager>();
        std::shared_ptr<Physics2DBody> body = physics2DManager.CreateBody(gameObject);
        body->SetType(Physics2DBody::Type::Dynamic);
        return std::make_unique<Rigidbody2D>(std::make_unique<Impl>(Guid::New(), gameObject, body));
    }

    Rigidbody2D::~Rigidbody2D() = default;

    Rigidbody2D::Rigidbody2D(std::unique_ptr<Impl> impl)
        : Component(*impl),
          impl(std::move(impl))
    {
    }

    Rigidbody2D::Rigidbody2D(Rigidbody2D&& other) noexcept = default;

    Rigidbody2D& Rigidbody2D::operator=(Rigidbody2D&& rhs) noexcept = default;

    Vector2F Rigidbody2D::GetVelocity() const
    {
        return impl->GetVelocity();
    }

    void Rigidbody2D::SetVelocity(const Vector2F& value)
    {
        return impl->SetVelocity(value);
    }

    float Rigidbody2D::GetAngularVelocity() const
    {
        return impl->GetAngularVelocity();
    }

    void Rigidbody2D::SetAngularVelocity(const float value)
    {
        return impl->SetAngularVelocity(value);
    }

    void Rigidbody2D::AddForce(const Vector2F& force)
    {
        impl->AddForce(force);
    }

    void Rigidbody2D::AddForce(const Vector2F& force, const Vector2F& point)
    {
        impl->AddForce(force, point);
    }

    void Rigidbody2D::AddTorque(const float torque)
    {
        impl->AddTorque(torque);
    }

    void Rigidbody2D::OnUpdate()
    {
        impl->OnUpdate();
    }
}
