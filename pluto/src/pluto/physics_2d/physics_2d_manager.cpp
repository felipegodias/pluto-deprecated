#include "pluto/physics_2d/physics_2d_manager.h"
#include "pluto/physics_2d/bodies/physics_2d_circle_body.h"
#include "pluto/memory/resource.h"
#include "pluto/scene/game_object.h"
#include "pluto/scene/components/transform.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/quaternion.h"
#include "pluto/guid.h"

#include <Box2D/Box2D.h>

#include <unordered_map>

namespace pluto
{
    class Physics2DManager::Impl
    {
        std::unique_ptr<b2World> world;
        std::unordered_map<Guid, std::unique_ptr<Physics2DBody>> bodies;
        Physics2DCircleBody::Factory* circleBodyFactory;

    public:
        explicit Impl(Physics2DCircleBody::Factory& circleBodyFactory)
            : circleBodyFactory(&circleBodyFactory)
        {
            world = std::make_unique<b2World>(b2Vec2{0, -9.81f});
        }

        Physics2DBody& Physics2DManager::CreateCircleBody(const Resource<GameObject>& gameObject, const float radius)
        {
            Resource<Transform> transform = gameObject->GetTransform();
            const Vector3F position = transform->GetPosition();
            const Vector3F eulerAngles = transform->GetRotation().GetEulerAngles();
            std::unique_ptr<Physics2DBody> body = circleBodyFactory->Create({position.x, position.z}, eulerAngles.z,
                                                                            radius);

            Physics2DBody* bodyPtr = body.get();

            bodies.emplace(gameObject.GetObjectId(), std::move(body));

            return *bodyPtr;
        }

        void DestroyBody(const Resource<GameObject>& gameObject)
        {
        }

        bool HasBody(const Resource<GameObject>& gameObject)
        {
        }

        Physics2DBody& GetBody(const Resource<GameObject>& gameObject)
        {
        }

        void* GetWorld() const
        {
            return world.get();
        }
    };

    Physics2DManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Physics2DManager> Physics2DManager::Factory::Create() const
    {
    }

    Physics2DManager::~Physics2DManager() = default;

    Physics2DManager::Physics2DManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    Physics2DManager::Physics2DManager(Physics2DManager&& other) noexcept = default;

    Physics2DManager& Physics2DManager::operator=(Physics2DManager&& other) noexcept = default;

    Physics2DBody& Physics2DManager::CreateCircleBody(const Resource<GameObject>& gameObject, float radius)
    {
    }

    void Physics2DManager::DestroyBody(const Resource<GameObject>& gameObject)
    {
    }

    bool Physics2DManager::HasBody(const Resource<GameObject>& gameObject)
    {
    }

    Physics2DBody& Physics2DManager::GetBody(const Resource<GameObject>& gameObject)
    {
    }

    void* Physics2DManager::GetWorld() const
    {
    }
}
