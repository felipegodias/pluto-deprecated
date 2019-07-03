#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>

#include <pluto/di/di_container.h>

namespace pluto
{
    class SceneManager::Impl
    {
    private:
        std::unique_ptr<Scene> activeScene;
        const Scene::Factory& sceneFactory;

    public:
        explicit Impl(const Scene::Factory& sceneFactory) : sceneFactory(sceneFactory)
        {
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
    };

    SceneManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<SceneManager> SceneManager::Factory::Create() const
    {
        auto& sceneFactory = diContainer.GetSingleton<Scene::Factory>();
        return std::make_unique<SceneManager>(std::make_unique<Impl>(sceneFactory));
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
