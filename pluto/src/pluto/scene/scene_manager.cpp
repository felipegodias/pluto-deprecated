#include <pluto/scene/scene_manager.h>

#include <pluto/event/event_manager.h>
#include <pluto/log/log_manager.h>

#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>

#include <pluto/service/service_collection.h>

#include "pluto/physics_2d/events/on_early_fixed_update_event.h"
#include "pluto/physics_2d/events/on_fixed_update_event.h"
#include "pluto/physics_2d/events/on_late_fixed_update_event.h"

#include "pluto/scene/events/on_early_update_event.h"
#include "pluto/scene/events/on_update_event.h"
#include "pluto/scene/events/on_late_update_event.h"

#include "pluto/render/events/on_render_event.h"
#include "pluto/render/events/on_post_render_event.h"
#include "pluto/render/events/on_pre_render_event.h"

#include "pluto/simulation/events/on_main_loop_end.h"

namespace pluto
{
    class SceneManager::Impl
    {
        std::unique_ptr<Scene> activeScene;

        const Scene::Factory& sceneFactory;
        const GameObject::Factory& gameObjectFactory;

        Guid onEarlyFixedUpdateEventListenerId;
        Guid onFixedUpdateEventListenerId;
        Guid onLateFixedUpdateEventListenerId;
        Guid onEarlyUpdateEventListenerId;
        Guid onUpdateEventListenerId;
        Guid onLateUpdateEventListenerId;
        Guid onPreRenderEventListenerId;
        Guid onRenderEventListenerId;
        Guid onPostRenderEventListenerId;
        Guid onMainLoopEndEventListenerId;

        EventManager& eventManager;
        LogManager& logManager;

    public:
        ~Impl()
        {
            activeScene->Destroy();
            activeScene->Cleanup();

            eventManager.Unsubscribe<OnEarlyFixedUpdateEvent>(onEarlyFixedUpdateEventListenerId);
            eventManager.Unsubscribe<OnFixedUpdateEvent>(onFixedUpdateEventListenerId);
            eventManager.Unsubscribe<OnLateFixedUpdateEvent>(onLateFixedUpdateEventListenerId);
            eventManager.Unsubscribe<OnEarlyUpdateEvent>(onEarlyUpdateEventListenerId);
            eventManager.Unsubscribe<OnUpdateEvent>(onUpdateEventListenerId);
            eventManager.Unsubscribe<OnLateUpdateEvent>(onLateUpdateEventListenerId);
            eventManager.Unsubscribe<OnPreRenderEvent>(onPreRenderEventListenerId);
            eventManager.Unsubscribe<OnRenderEvent>(onRenderEventListenerId);
            eventManager.Unsubscribe<OnPostRenderEvent>(onPostRenderEventListenerId);
            eventManager.Unsubscribe<OnMainLoopEndEvent>(onMainLoopEndEventListenerId);

            logManager.LogInfo("SceneManager terminated!");
        }

        Impl(const Scene::Factory& sceneFactory, const GameObject::Factory& gameObjectFactory,
             EventManager& eventManager, LogManager& logManager)
            : sceneFactory(sceneFactory),
              gameObjectFactory(gameObjectFactory),
              eventManager(eventManager),
              logManager(logManager)
        {
            onEarlyFixedUpdateEventListenerId = eventManager.Subscribe<OnEarlyFixedUpdateEvent>(
                std::bind(&Impl::OnEarlyFixedUpdate, this, std::placeholders::_1));

            onFixedUpdateEventListenerId = eventManager.Subscribe<OnFixedUpdateEvent>(
                std::bind(&Impl::OnFixedUpdate, this, std::placeholders::_1));

            onLateFixedUpdateEventListenerId = eventManager.Subscribe<OnLateFixedUpdateEvent>(
                std::bind(&Impl::OnLateFixedUpdate, this, std::placeholders::_1));

            onEarlyUpdateEventListenerId = eventManager.Subscribe<OnEarlyUpdateEvent>(
                std::bind(&Impl::OnEarlyUpdate, this, std::placeholders::_1));

            onUpdateEventListenerId = eventManager.Subscribe<OnUpdateEvent>(
                std::bind(&Impl::OnUpdate, this, std::placeholders::_1));

            onLateUpdateEventListenerId = eventManager.Subscribe<OnLateUpdateEvent>(
                std::bind(&Impl::OnLateUpdate, this, std::placeholders::_1));

            onPreRenderEventListenerId = eventManager.Subscribe<OnPreRenderEvent>(
                std::bind(&Impl::OnPreRender, this, std::placeholders::_1));

            onRenderEventListenerId = eventManager.Subscribe<OnRenderEvent>(
                std::bind(&Impl::OnRender, this, std::placeholders::_1));

            onPostRenderEventListenerId = eventManager.Subscribe<OnPostRenderEvent>(
                std::bind(&Impl::OnPostRender, this, std::placeholders::_1));

            onMainLoopEndEventListenerId = eventManager.Subscribe<OnMainLoopEndEvent>(
                std::bind(&Impl::OnMainLoopEnd, this, std::placeholders::_1));

            logManager.LogInfo("SceneManager initialized!");
        }

        Scene& GetActiveScene() const
        {
            return *activeScene;
        }

        void LoadEmptyScene()
        {
            if (activeScene != nullptr)
            {
                activeScene->Destroy();
            }

            activeScene = sceneFactory.Create();
        }

    private:
        void OnEarlyFixedUpdate(const OnEarlyFixedUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnEarlyFixedUpdate();
            }
        }

        void OnFixedUpdate(const OnFixedUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnFixedUpdate();
            }
        }

        void OnLateFixedUpdate(const OnLateFixedUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnLateFixedUpdate();
            }
        }

        void OnEarlyUpdate(const OnEarlyUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnEarlyUpdate();
            }
        }

        void OnUpdate(const OnUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnUpdate();
            }
        }

        void OnLateUpdate(const OnLateUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnLateUpdate();
            }
        }

        void OnPreRender(const OnPreRenderEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnPreRender();
            }
        }

        void OnRender(const OnRenderEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnRender();
            }
        }

        void OnPostRender(const OnPostRenderEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->OnPostRender();
            }
        }

        void OnMainLoopEnd(const OnMainLoopEndEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->Cleanup();
            }
        }
    };

    SceneManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<SceneManager> SceneManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        Scene::Factory& sceneFactory = serviceCollection.GetFactory<Scene>();
        GameObject::Factory& gameObjectFactory = serviceCollection.GetFactory<GameObject>();
        auto& eventManager = serviceCollection.GetService<EventManager>();
        auto& logManager = serviceCollection.GetService<LogManager>();
        return std::make_unique<SceneManager>(
            std::make_unique<Impl>(sceneFactory, gameObjectFactory, eventManager, logManager));
    }

    SceneManager::SceneManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    SceneManager::SceneManager(SceneManager&& other) noexcept
        : impl(std::move(other.impl))
    {
    }

    SceneManager::~SceneManager() = default;

    SceneManager& SceneManager::operator=(SceneManager&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    Scene& SceneManager::GetActiveScene() const
    {
        return impl->GetActiveScene();
    }

    void SceneManager::LoadEmptyScene()
    {
        return impl->LoadEmptyScene();
    }
}
