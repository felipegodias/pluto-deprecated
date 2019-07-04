#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>

#include <pluto/guid.h>

#include <pluto/di/di_container.h>

namespace pluto
{
    class Scene::Impl
    {
    private:
        Guid guid;
        std::unique_ptr<GameObject> rootGameObject;

    public:
        Impl(Guid guid, std::unique_ptr<GameObject> rootGameObject) : guid(std::move(guid)),
                                                                      rootGameObject(std::move(rootGameObject))
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        GameObject& GetRootGameObject() const
        {
            return *rootGameObject;
        }

        void Destroy()
        {
            rootGameObject->Destroy();
            rootGameObject.reset();
        }
    };

    Scene::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<Scene> Scene::Factory::Create() const
    {
        const auto& gameObjectFactory = diContainer.GetSingleton<GameObject::Factory>();
        auto rootGameObject = gameObjectFactory.Create();
        return std::make_unique<Scene>(std::make_unique<Impl>(Guid::New(), std::move(rootGameObject)));
    }

    Scene::Scene(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    Scene::Scene(Scene&& other) noexcept : impl(std::move(other.impl))
    {
    }

    Scene::~Scene() = default;

    Scene& Scene::operator=(Scene&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    const Guid& Scene::GetId() const
    {
        return impl->GetId();
    }

    GameObject& Scene::GetRootGameObject() const
    {
        return impl->GetRootGameObject();
    }

    void Scene::Destroy()
    {
        impl->Destroy();
    }
}
