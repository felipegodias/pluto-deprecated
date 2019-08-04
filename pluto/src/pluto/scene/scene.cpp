#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/components/transform.h>

#include <pluto/guid.h>

#include <pluto/service/service_collection.h>
#include <pluto/memory/memory_manager.h>

#include <list>

namespace pluto
{
    class Scene::Impl
    {
        Guid guid;
        Resource<GameObject> root;
        std::list<GameObject*> gameObjects;

        MemoryManager* memoryManager;
        GameObject::Factory* gameObjectFactory;

    public:
        Impl(const Guid& guid, std::unique_ptr<GameObject> rootGameObject, MemoryManager& memoryManager,
             GameObject::Factory& gameObjectFactory)
            : guid(guid),
              memoryManager(&memoryManager),
              gameObjectFactory(&gameObjectFactory)
        {
            root = ResourceUtils::Cast<GameObject>(memoryManager.Add(gameObjectFactory.Create()));
            gameObjects.push_back(root.Get());
        }

        const Guid& GetId() const
        {
            return guid;
        }

        Resource<GameObject> GetRootGameObject() const
        {
            return root;
        }

        Resource<GameObject> CreateGameObject(Transform& parent, const std::string& name)
        {
            Resource<GameObject> gameObjectResource = ResourceUtils::Cast<GameObject>(
                memoryManager->Add(gameObjectFactory->Create()));

            GameObject* gameObject = gameObjectResource.Get();
            gameObjects.push_back(gameObject);

            gameObject->SetName(name);
            gameObject->GetTransform().SetParent(parent);
            return gameObjectResource;
        }

        void Destroy()
        {
            gameObjects.front()->Destroy();
        }

        void OnUpdate(const uint32_t currentFrame)
        {
            gameObjects.front()->OnUpdate(currentFrame);
        }

        void OnCleanup()
        {
            auto it = gameObjects.begin();
            while (it != gameObjects.end())
            {
                GameObject* go = *it;
                if (go->IsDestroyed())
                {
                    memoryManager->Remove(*go);
                    gameObjects.erase(it++);
                }
                else
                {
                    ++it;
                }
            }
        }
    };

    Scene::Factory::Factory(ServiceCollection& diContainer)
        : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<Scene> Scene::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        auto& gameObjectFactory = serviceCollection.GetService<GameObject::Factory>();
        auto rootGameObject = gameObjectFactory.Create();
        rootGameObject->SetName("root");
        return std::make_unique<Scene>(
            std::make_unique<Impl>(Guid::New(), std::move(rootGameObject), memoryManager, gameObjectFactory));
    }

    Scene::Scene(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    Scene::Scene(Scene&& other) noexcept
        : impl(std::move(other.impl))
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

    Resource<GameObject> Scene::GetRootGameObject() const
    {
        return impl->GetRootGameObject();
    }

    Resource<GameObject> Scene::CreateGameObject()
    {
        return impl->CreateGameObject(GetRootGameObject()->GetTransform(), "New Game Object");
    }

    Resource<GameObject> Scene::CreateGameObject(const std::string& name)
    {
        return impl->CreateGameObject(GetRootGameObject()->GetTransform(), name);
    }

    Resource<GameObject> Scene::CreateGameObject(Transform& parent)
    {
        return impl->CreateGameObject(parent, "New Game Object");
    }

    Resource<GameObject> Scene::CreateGameObject(Transform& parent, const std::string& name)
    {
        return impl->CreateGameObject(parent, name);
    }

    void Scene::Destroy()
    {
        impl->Destroy();
    }

    void Scene::OnUpdate(const uint32_t currentFrame)
    {
        impl->OnUpdate(currentFrame);
    }

    void Scene::OnCleanup()
    {
        impl->OnCleanup();
    }
}
