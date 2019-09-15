#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/components/transform.h>

#include <pluto/guid.h>

#include <pluto/service/service_collection.h>
#include <pluto/event/event_manager.h>
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
        Impl(const Guid& guid, MemoryManager& memoryManager, GameObject::Factory& gameObjectFactory)
            : guid(guid),
              memoryManager(&memoryManager),
              gameObjectFactory(&gameObjectFactory)
        {
            root = ResourceUtils::Cast<GameObject>(memoryManager.Add(gameObjectFactory.Create()));
            root->SetName("root");
            root->AddComponent<Transform>();
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

        Resource<GameObject> CreateGameObject(const Resource<Transform>& parent, const std::string& name)
        {
            Resource<GameObject> gameObjectResource = ResourceUtils::Cast<GameObject>(
                memoryManager->Add(gameObjectFactory->Create()));

            GameObject* gameObject = gameObjectResource.Get();
            gameObject->AddComponent<Transform>();
            gameObjects.push_back(gameObject);

            gameObject->SetName(name);
            gameObject->GetTransform()->SetParent(parent);
            return gameObjectResource;
        }

        void Destroy()
        {
            gameObjects.front()->Destroy();
        }

        void OnEarlyFixedUpdate()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnEarlyFixedUpdate();
            }
        }

        void OnFixedUpdate()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnFixedUpdate();
            }
        }

        void OnLateFixedUpdate()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnLateFixedUpdate();
            }
        }

        void OnEarlyUpdate()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnEarlyUpdate();
            }
        }

        void OnUpdate()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnUpdate();
            }
        }

        void OnLateUpdate()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnLateUpdate();
            }
        }

        void OnPreRender()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnPreRender();
            }
        }

        void OnRender()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnRender();
            }
        }

        void OnPostRender()
        {
            for (auto& gameObject : gameObjects)
            {
                gameObject->OnPostRender();
            }
        }

        void Cleanup()
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

    Scene::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<Scene> Scene::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        GameObject::Factory& gameObjectFactory = serviceCollection.GetFactory<GameObject>();

        return std::make_unique<Scene>(std::make_unique<Impl>(Guid::New(), memoryManager, gameObjectFactory));
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

    Resource<GameObject> Scene::CreateGameObject(const Resource<Transform>& parent)
    {
        return impl->CreateGameObject(parent, "New Game Object");
    }

    Resource<GameObject> Scene::CreateGameObject(const Resource<Transform>& parent, const std::string& name)
    {
        return impl->CreateGameObject(parent, name);
    }

    void Scene::Destroy()
    {
        impl->Destroy();
    }

    void Scene::OnEarlyFixedUpdate()
    {
        impl->OnEarlyFixedUpdate();
    }

    void Scene::OnFixedUpdate()
    {
        impl->OnFixedUpdate();
    }

    void Scene::OnLateFixedUpdate()
    {
        impl->OnLateFixedUpdate();
    }

    void Scene::OnEarlyUpdate()
    {
        impl->OnEarlyUpdate();
    }

    void Scene::OnUpdate()
    {
        impl->OnUpdate();
    }

    void Scene::OnLateUpdate()
    {
        impl->OnLateUpdate();
    }

    void Scene::OnPreRender()
    {
        impl->OnPreRender();
    }

    void Scene::OnRender()
    {
        impl->OnRender();
    }

    void Scene::OnPostRender()
    {
        impl->OnPostRender();
    }

    void Scene::Cleanup()
    {
        impl->Cleanup();
    }
}
