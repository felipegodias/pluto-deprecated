#include <pluto/scene/scene_manager.h>

#include <pluto/event/event_manager.h>
#include <pluto/simulation/on_update_event.h>
#include <pluto/log/log_manager.h>

#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>

#include <pluto/guid.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    class SceneManager::Impl
    {
    private:
        std::unique_ptr<Scene> activeScene;

        Guid onUpdateEventId;

        const Scene::Factory& sceneFactory;
        EventManager& eventManager;
        LogManager& logManager;
        uint32_t currentFrame;

    public:
        Impl(const Scene::Factory& sceneFactory, EventManager& eventManager, LogManager& logManager) :
            sceneFactory(sceneFactory), eventManager(eventManager), logManager(logManager)
        {
            static Impl* instance = this;
            onUpdateEventId = eventManager.Subscribe<OnUpdateEvent>([&](const OnUpdateEvent& evt)
            {
                instance->OnUpdate(evt);
            });

            logManager.LogInfo("SceneManager initialized!");
        }

        ~Impl()
        {
            eventManager.Unsubscribe<OnUpdateEvent>(onUpdateEventId);
            logManager.LogInfo("SceneManager terminated!");
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
        void OnUpdate(const OnUpdateEvent& evt)
        {
            if (activeScene != nullptr)
            {
                activeScene->GetRootGameObject().OnUpdate(currentFrame);
            }
            ++currentFrame;
        }
    };

    SceneManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<SceneManager> SceneManager::Factory::Create() const
    {
        auto& sceneFactory = diContainer.GetSingleton<Scene::Factory>();
        auto& eventManager = diContainer.GetSingleton<EventManager>();
        auto& logManager = diContainer.GetSingleton<LogManager>();
        return std::make_unique<SceneManager>(std::make_unique<Impl>(sceneFactory, eventManager, logManager));
    }

    SceneManager::SceneManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    SceneManager::SceneManager(SceneManager&& other) noexcept : impl(std::move(other.impl))
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
