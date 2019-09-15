#include "pluto/physics_2d/physics_2d_manager.h"
#include "pluto/physics_2d/physics_2d_body.h"
#include "pluto/physics_2d/events/on_fixed_update_event.h"
#include "pluto/physics_2d/collision_2d.h"
#include "pluto/physics_2d/components/collider_2d.h"

#include "pluto/debug/assert.h"

#include "pluto/service/service_collection.h"
#include "pluto/log/log_manager.h"
#include "pluto/event/event_manager.h"
#include "pluto/memory/memory_manager.h"

#include "pluto/render/events/on_pre_render_event.h"

#include "pluto/render/render_manager.h"

#include "pluto/memory/resource.h"

#include "pluto/scene/components/transform.h"
#include "pluto/scene/game_object.h"

#include "pluto/math/color.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/quaternion.h"

#include "pluto/guid.h"

#include <Box2D/Box2D.h>

#include <unordered_map>
#include <iostream>

namespace pluto
{
    class PhysicsContactListener final : public b2ContactListener
    {
        MemoryManager* memoryManager;
        Collision2D::Factory* collisionFactory;

    public:
        explicit PhysicsContactListener(MemoryManager& memoryManager, Collision2D::Factory& collisionFactory)
            : memoryManager(&memoryManager),
              collisionFactory(&collisionFactory)
        {
        }

        void BeginContact(b2Contact* contact) override
        {
            const Resource<Collider2D> colliderA = GetCollider(*contact->GetFixtureA());
            const Resource<Collider2D> colliderB = GetCollider(*contact->GetFixtureB());

            const std::vector<Vector2F> contactPoints = GetContactPoints(*contact->GetManifold());

            const std::unique_ptr<Collision2D> collisionA = collisionFactory->Create(
                colliderA, colliderB, contactPoints);
            const std::unique_ptr<Collision2D> collisionB = collisionFactory->Create(
                colliderB, colliderA, contactPoints);

            colliderA->GetGameObject()->OnCollision2DBegin(*collisionA);
            colliderB->GetGameObject()->OnCollision2DBegin(*collisionB);
        }

        void EndContact(b2Contact* contact) override
        {
            const Resource<Collider2D> colliderA = GetCollider(*contact->GetFixtureA());
            const Resource<Collider2D> colliderB = GetCollider(*contact->GetFixtureB());

            const std::vector<Vector2F> contactPoints = GetContactPoints(*contact->GetManifold());

            const std::unique_ptr<Collision2D> collisionA = collisionFactory->Create(
                colliderA, colliderB, contactPoints);
            const std::unique_ptr<Collision2D> collisionB = collisionFactory->Create(
                colliderB, colliderA, contactPoints);

            colliderA->GetGameObject()->OnCollision2DEnd(*collisionA);
            colliderB->GetGameObject()->OnCollision2DEnd(*collisionB);
        }

    private:
        Resource<Collider2D> GetCollider(const b2Fixture& fixture) const
        {
            Guid* colliderId = reinterpret_cast<Guid*>(fixture.GetUserData());
            return ResourceUtils::Cast<Collider2D>(memoryManager->Get(*colliderId));
        }

        static std::vector<Vector2F> GetContactPoints(const b2Manifold& manifold)
        {
            std::vector<Vector2F> contactPoints;
            for (int i = 0; i < manifold.pointCount; ++i)
            {
                const b2Vec2 point = manifold.points[i].localPoint;
                contactPoints.emplace_back(point.x, point.y);
            }
            return contactPoints;
        }
    };

    class PhysicsDebugDrawer final : public b2Draw
    {
        RenderManager* renderManager;

    public:
        explicit PhysicsDebugDrawer(RenderManager& renderManager)
            : renderManager(&renderManager)
        {
        }

        void DrawPolygon(const b2Vec2* vertices, const int32 vertexCount, const b2Color& color) override
        {
            std::vector<Vector2F> points;
            for (int i = 0; i < vertexCount; ++i)
            {
                points.emplace_back(vertices[i].x, vertices[i].y);
            }
            renderManager->DrawPolygonGizmo(points, ToPlutoColor(color));
        }

        void DrawSolidPolygon(const b2Vec2* vertices, const int32 vertexCount, const b2Color& color) override
        {
            DrawPolygon(vertices, vertexCount, color);
        }

        void DrawCircle(const b2Vec2& center, const float32 radius, const b2Color& color) override
        {
            renderManager->DrawCircleGizmo({center.x, center.y}, radius, ToPlutoColor(color));
        }

        void DrawSolidCircle(const b2Vec2& center, const float32 radius, const b2Vec2& axis,
                             const b2Color& color) override
        {
            DrawCircle(center, radius, color);
        }

        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
        {
        }

        void DrawTransform(const b2Transform& xf) override
        {
        }

        static Color ToPlutoColor(const b2Color& color)
        {
            return {
                static_cast<uint8_t>(color.r * 255), static_cast<uint8_t>(color.g * 255),
                static_cast<uint8_t>(color.b * 255), static_cast<uint8_t>(color.a * 255)
            };
        }
    };

    class Physics2DManager::Impl
    {
        static inline const b2Vec2 GRAVITY = {0, -9.81f};

        std::unique_ptr<b2World> world;
        std::unique_ptr<PhysicsContactListener> contactListener;
        std::unique_ptr<PhysicsDebugDrawer> debugDrawer;
        std::unordered_map<Guid, std::shared_ptr<Physics2DBody>> bodies;

        Guid onFixedUpdateEventListenerId;
        Guid onPreRenderEventListenerId;

        LogManager* logManager;
        EventManager* eventManager;
        Physics2DBody::Factory* bodyFactory;

    public:
        ~Impl()
        {
            eventManager->Unsubscribe<OnFixedUpdateEvent>(onFixedUpdateEventListenerId);
            eventManager->Unsubscribe<OnPreRenderEvent>(onPreRenderEventListenerId);
            logManager->LogInfo("Physics2DManager terminated!");
        }

        Impl(LogManager& logManager, EventManager& eventManager, Physics2DBody::Factory& bodyFactory,
             std::unique_ptr<PhysicsContactListener> contactListener, std::unique_ptr<PhysicsDebugDrawer> debugDrawer)
            : contactListener(std::move(contactListener)),
              debugDrawer(std::move(debugDrawer)),
              logManager(&logManager),
              eventManager(&eventManager),
              bodyFactory(&bodyFactory)

        {
            world = std::make_unique<b2World>(GRAVITY);
            this->debugDrawer->SetFlags(b2Draw::e_shapeBit);
            world->SetContactListener(this->contactListener.get());
            world->SetDebugDraw(this->debugDrawer.get());

            onFixedUpdateEventListenerId = eventManager.Subscribe(*this, &Impl::OnFixedUpdate);

            onPreRenderEventListenerId = eventManager.Subscribe(*this, &Impl::OnPreRender);

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

            std::shared_ptr<Physics2DBody> body = bodyFactory->Create(gameObject, {position.x, position.y},
                                                                      eulerAngles.z);

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

        void OnFixedUpdate(const OnFixedUpdateEvent& evt)
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

            world->Step(0.02f, 6, 2);
        }

        void OnPreRender(const OnPreRenderEvent& evt)
        {
            world->DrawDebugData();
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
        auto& eventManager = serviceCollection.GetService<EventManager>();
        Physics2DBody::Factory& bodyFactory = serviceCollection.GetFactory<Physics2DBody>();

        auto& renderManager = serviceCollection.GetService<RenderManager>();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        Collision2D::Factory& collisionFactory = serviceCollection.GetFactory<Collision2D>();

        return std::make_unique<Physics2DManager>(std::make_unique<Impl>(
            logManager, eventManager, bodyFactory,
            std::make_unique<PhysicsContactListener>(memoryManager, collisionFactory),
            std::make_unique<PhysicsDebugDrawer>(renderManager)));
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
}
