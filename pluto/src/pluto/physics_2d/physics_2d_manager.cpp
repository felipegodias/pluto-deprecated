#include "pluto/physics_2d/physics_2d_manager.h"
#include "pluto/physics_2d/physics_2d_body.h"

#include "pluto/debug/assert.h"

#include "pluto/service/service_collection.h"
#include "pluto/log/log_manager.h"

#include "pluto/memory/resource.h"
#include "pluto/scene/game_object.h"
#include "pluto/scene/components/transform.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/quaternion.h"

#include "pluto/guid.h"

#include <Box2D/Box2D.h>

#include <unordered_map>

namespace pluto
{
    class Physics2DManager::Impl
    {
        static inline const b2Vec2 GRAVITY = {0, -9.81f};

        std::unique_ptr<b2World> world;
        std::unordered_map<Guid, std::shared_ptr<Physics2DBody>> bodies;

        LogManager* logManager;
        Physics2DBody::Factory* bodyFactory;

    public:
        ~Impl()
        {
            logManager->LogInfo("Physics2DManager terminated!");
        }

        Impl(LogManager& logManager, Physics2DBody::Factory& bodyFactory)
            : logManager(&logManager),
              bodyFactory(&bodyFactory)

        {
            world = std::make_unique<b2World>(GRAVITY);
            logManager.LogInfo("Physics2DManager initialized!");
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& other) = delete;

        Impl& operator=(Impl&& other) noexcept = default;

        bool HasBody(const Resource<GameObject>& gameObject) const
        {
            return bodies.find(gameObject.GetObjectId()) != bodies.end();
        }

        std::shared_ptr<Physics2DBody> GetBody(const Resource<GameObject>& gameObject) const
        {
            ASSERT_THAT_IS_TRUE(HasBody(gameObject));
            return bodies.find(gameObject.GetObjectId())->second;
        }

        std::shared_ptr<Physics2DBody> CreateBody(const Resource<GameObject>& gameObject)
        {
            ASSERT_THAT_IS_FALSE(HasBody(gameObject));
            Resource<Transform> transform = gameObject->GetTransform();

            const Vector3F position = transform->GetPosition();
            const Vector3F eulerAngles = transform->GetRotation().GetEulerAngles();

            std::shared_ptr<Physics2DBody> body = bodyFactory->Create({position.x, position.y}, eulerAngles.z);
            bodies.emplace(gameObject.GetObjectId(), body);
            return body;
        }

        std::shared_ptr<Physics2DBody> GetOrCreateBody(const Resource<GameObject>& gameObject)
        {
            return HasBody(gameObject) ? GetBody(gameObject) : CreateBody(gameObject);
        }

        void* GetWorld() const
        {
            return world.get();
        }

        void MainLoop(const float deltaTime)
        {
            for (auto it = bodies.begin(); it != bodies.end();)
            {
                if (it->second.use_count() == 1)
                {
                    bodies.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            world->Step(deltaTime, 6, 2);
        }
    };

    Physics2DManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Physics2DManager> Physics2DManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetService<LogManager>();
        Physics2DBody::Factory& bodyFactory = serviceCollection.GetFactory<Physics2DBody>();
        return std::make_unique<Physics2DManager>(std::make_unique<Impl>(logManager, bodyFactory));
    }

    Physics2DManager::~Physics2DManager() = default;

    Physics2DManager::Physics2DManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    Physics2DManager::Physics2DManager(Physics2DManager&& other) noexcept = default;

    Physics2DManager& Physics2DManager::operator=(Physics2DManager&& rhs) noexcept = default;

    bool Physics2DManager::HasBody(const Resource<GameObject>& gameObject) const
    {
        return impl->HasBody(gameObject);
    }

    std::shared_ptr<Physics2DBody> Physics2DManager::GetBody(const Resource<GameObject>& gameObject) const
    {
        return impl->GetBody(gameObject);
    }

    std::shared_ptr<Physics2DBody> Physics2DManager::CreateBody(const Resource<GameObject>& gameObject)
    {
        return impl->CreateBody(gameObject);
    }

    std::shared_ptr<Physics2DBody> Physics2DManager::GetOrCreateBody(const Resource<GameObject>& gameObject)
    {
        return impl->GetOrCreateBody(gameObject);
    }

    void* Physics2DManager::GetWorld() const
    {
        return impl->GetWorld();
    }

    void Physics2DManager::MainLoop(const float deltaTime)
    {
        impl->MainLoop(deltaTime);
    }
}
