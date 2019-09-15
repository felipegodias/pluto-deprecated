#include "pluto/physics_2d/components/box_collider_2d.h"
#include "pluto/physics_2d/components/collider_2d.impl.hpp"
#include "pluto/physics_2d/shapes/physics_2d_box_shape.h"
#include "pluto/physics_2d/physics_2d_manager.h"

#include "pluto/service/service_collection.h"

namespace pluto
{
    class BoxCollider2D::Impl : public Collider2D::Impl
    {
        std::unique_ptr<Physics2DBoxShape> shape;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject, std::unique_ptr<Physics2DBoxShape> shape,
             const std::shared_ptr<Physics2DBody>& body)
            : Collider2D::Impl(guid, gameObject, *shape, body),
              shape(std::move(shape))
        {
        }

        Vector2F GetSize() const
        {
            return shape->GetSize();
        }

        void SetSize(const Vector2F& value)
        {
            shape->SetSize(value);
        }
    };

    BoxCollider2D::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> BoxCollider2D::Factory::Create(const Resource<GameObject>& gameObject) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& physics2DManager = serviceCollection.GetService<Physics2DManager>();
        std::shared_ptr<Physics2DBody> body = physics2DManager.GetOrCreateBody(gameObject);

        Guid colliderId = Guid::New();
        std::unique_ptr<Physics2DBoxShape> shape = body->CreateBoxShape(colliderId, Vector2F::ZERO, Vector2F::ONE);
        return std::make_unique<BoxCollider2D>(
            std::make_unique<Impl>(colliderId, gameObject, std::move(shape), body));
    }

    BoxCollider2D::~BoxCollider2D() = default;

    BoxCollider2D::BoxCollider2D(std::unique_ptr<Impl> impl)
        : Collider2D(*impl),
          impl(std::move(impl))
    {
    }

    BoxCollider2D::BoxCollider2D(BoxCollider2D&& other) noexcept = default;

    BoxCollider2D& BoxCollider2D::operator=(BoxCollider2D&& rhs) noexcept = default;

    Vector2F BoxCollider2D::GetSize() const
    {
        return impl->GetSize();
    }

    void BoxCollider2D::SetSize(const Vector2F& value)
    {
        impl->SetSize(value);
    }
}
