#include "pluto/physics_2d/collision_2d.h"

#include "pluto/memory/resource.h"
#include "pluto/physics_2d/components/collider_2d.h"
#include "pluto/math/vector2f.h"

#include <utility>
#include <vector>

namespace pluto
{
    class Collision2D::Impl
    {
        Resource<Collider2D> collider;
        Resource<Collider2D> otherCollider;

        std::vector<Vector2F> contactPoints;

    public:
        Impl(Resource<Collider2D> collider, Resource<Collider2D> otherCollider,
             std::vector<Vector2F> contactPoints)
            : collider(std::move(collider)),
              otherCollider(std::move(otherCollider)),
              contactPoints(std::move(contactPoints))
        {
        }

        Resource<Collider2D> GetCollider() const
        {
            return collider;
        }

        Resource<Collider2D> GetOtherCollider() const
        {
            return otherCollider;
        }

        const std::vector<Vector2F>& GetContactPoints() const
        {
            return contactPoints;
        }
    };

    Collision2D::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Collision2D> Collision2D::Factory::Create(const Resource<Collider2D>& collider,
                                                              const Resource<Collider2D>& otherCollider,
                                                              const std::vector<Vector2F>& contactPoints) const
    {
        return std::make_unique<Collision2D>(std::make_unique<Impl>(collider, otherCollider, contactPoints));
    }

    Collision2D::~Collision2D() = default;

    Collision2D::Collision2D(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    Collision2D::Collision2D(Collision2D&& other) noexcept = default;

    Collision2D& Collision2D::operator=(Collision2D&& rhs) noexcept = default;

    Resource<Collider2D> Collision2D::GetCollider() const
    {
        return impl->GetCollider();
    }

    Resource<Collider2D> Collision2D::GetOtherCollider() const
    {
        return impl->GetOtherCollider();
    }

    const std::vector<Vector2F>& Collision2D::GetContactPoints() const
    {
        return impl->GetContactPoints();
    }
}
