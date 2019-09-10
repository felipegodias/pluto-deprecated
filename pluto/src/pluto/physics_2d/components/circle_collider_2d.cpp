#include "pluto/physics_2d/components/circle_collider_2d.h"
#include "pluto/physics_2d/components/collider_2d.impl.hpp"
#include "pluto/physics_2d/shapes/physics_2d_circle_shape.h"
#include "pluto/physics_2d/physics_2d_manager.h"

#include "pluto/service/service_collection.h"

namespace pluto
{
    class CircleCollider2D::Impl : public Collider2D::Impl
    {
        std::unique_ptr<Physics2DCircleShape> shape;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject, std::unique_ptr<Physics2DCircleShape> shape,
             const std::shared_ptr<Physics2DBody>& body)
            : Collider2D::Impl(guid, gameObject, *shape, body),
              shape(std::move(shape))
        {
        }

        float GetRadius() const
        {
            return shape->GetRadius();
        }

        void SetRadius(const float value)
        {
            shape->SetRadius(value);
        }
    };

    CircleCollider2D::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> CircleCollider2D::Factory::Create(const Resource<GameObject>& gameObject) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& physics2DManager = serviceCollection.GetService<Physics2DManager>();
        std::shared_ptr<Physics2DBody> body = physics2DManager.GetOrCreateBody(gameObject);
        std::unique_ptr<Physics2DCircleShape> shape = body->CreateCircleShape(Vector2F::ZERO, 1);
        return std::make_unique<CircleCollider2D>(
            std::make_unique<Impl>(Guid::New(), gameObject, std::move(shape), body));
    }

    CircleCollider2D::~CircleCollider2D() = default;

    CircleCollider2D::CircleCollider2D(std::unique_ptr<Impl> impl)
        : Collider2D(*impl),
          impl(std::move(impl))
    {
    }

    CircleCollider2D::CircleCollider2D(CircleCollider2D&& other) noexcept = default;

    CircleCollider2D& CircleCollider2D::operator=(CircleCollider2D&& rhs) noexcept = default;

    float CircleCollider2D::GetRadius() const
    {
        return impl->GetRadius();
    }

    void CircleCollider2D::SetRadius(const float value)
    {
        impl->SetRadius(value);
    }
}
