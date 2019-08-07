#include <pluto/scene/game_object.h>
#include <pluto/scene/components/component.h>
#include <pluto/scene/components/transform.h>

#include <pluto/memory/resource.h>
#include <pluto/memory/memory_manager.h>

#include <pluto/service/service_collection.h>
#include <pluto/guid.h>
#include <pluto/exception.h>

#include <typeindex>
#include <unordered_map>

namespace pluto
{
    class GameObject::Impl
    {
        Guid guid;
        std::string name;
        Flags flags;

        Resource<Transform> transform;

        std::vector<Resource<Component>> components;

        uint32_t lastFrame;
        bool isDestroyed;

        MemoryManager* memoryManager;
        ServiceCollection* serviceCollection;

    public:
        Impl(const Guid& guid, MemoryManager& memoryManager, ServiceCollection& serviceCollection)
            : guid(guid),
              flags(Flags::None),
              transform(nullptr),
              lastFrame(0),
              isDestroyed(false),
              memoryManager(&memoryManager),
              serviceCollection(&serviceCollection)
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(const std::string& value)
        {
            name = value;
        }

        Flags GetFlags() const
        {
            return flags;
        }

        bool IsDestroyed() const
        {
            return isDestroyed;
        }

        Resource<Transform> GetTransform() const
        {
            return transform;
        }

        Resource<Component> AddComponent(const std::type_info& type)
        {
            const Component::Factory& factory = dynamic_cast<Component::Factory&>(serviceCollection->GetFactory(type));

            // TODO: FIX ME - GameObject does not exists when trying to add transform.
            const Resource<GameObject> gameObject = ResourceUtils::Cast<GameObject>(
                memoryManager->Get(guid));

            Resource<Component> component = ResourceUtils::Cast<Component>(
                memoryManager->Add(factory.Create(gameObject)));

            if (type == typeid(Transform))
            {
                transform = ResourceUtils::Cast<Transform>(component);
            }

            components.push_back(component);
            return component;
        }

        Resource<Component> GetComponent(const std::function<bool(const Component& component)>& predicate) const
        {
            for (auto& component : components)
            {
                if (predicate(*component.Get()))
                {
                    return component;
                }
            }
            return nullptr;
        }

        std::vector<Resource<Component>> GetComponents(
            const std::function<bool(const Component& component)>& predicate) const
        {
            std::vector<Resource<Component>> result;
            for (auto& component : components)
            {
                if (predicate(*component.Get()))
                {
                    result.emplace_back(component);
                }
            }
            return result;
        }

        Resource<Component> GetComponentInChildren(
            const std::function<bool(const Component& component)>& predicate) const
        {
            Resource<Component> result = GetComponent(predicate);
            if (result != nullptr)
            {
                return result;
            }

            for (auto& it : GetTransform()->GetChildren())
            {
                result = it->GetGameObject()->GetComponentInChildren(predicate);
                if (result != nullptr)
                {
                    return result;
                }
            }
            return nullptr;
        }

        std::vector<Resource<Component>> GetComponentsInChildren(
            const std::function<bool(const Component& component)>& predicate) const
        {
            std::vector<Resource<Component>> result = GetComponents(predicate);
            for (auto it : GetTransform()->GetChildren())
            {
                auto componentsFromChild = it->GetGameObject()->GetComponentsInChildren(predicate);
                result.insert(result.end(), componentsFromChild.begin(), componentsFromChild.end());
            }
            return result;
        }

        void Destroy()
        {
            if (isDestroyed)
            {
                return;
            }

            isDestroyed = true;

            for (auto& component : components)
            {
                component->OnDestroy();
            }

            for (auto& it : transform->GetChildren())
            {
                it->GetGameObject()->Destroy();
            }
        }

        void OnUpdate(const uint32_t currentFrame)
        {
            // Only run update if the object was not destroyed or already updated in the current frame.
            if (isDestroyed || lastFrame >= currentFrame)
            {
                return;
            }

            lastFrame = currentFrame;
            for (auto& component : components)
            {
                component->OnUpdate();
            }

            if (!isDestroyed)
            {
                // Uses a copy of the children because the parent can be changed while in update.
                std::vector<Resource<Transform>> children = transform->GetChildren();
                for (auto& it : children)
                {
                    it->GetGameObject()->OnUpdate(currentFrame);
                }
            }
        }
    };

    GameObject::Factory::Factory(ServiceCollection& diContainer)
        : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<GameObject> GameObject::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        auto gameObject = std::make_unique<GameObject>(
            std::make_unique<Impl>(Guid::New(), memoryManager, serviceCollection));
        return gameObject;
    }

    GameObject::~GameObject() = default;

    GameObject::GameObject(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    GameObject::GameObject(GameObject&& other) noexcept = default;

    GameObject& GameObject::operator=(GameObject&& rhs) noexcept = default;

    const Guid& GameObject::GetId() const
    {
        return impl->GetId();
    }

    const std::string& GameObject::GetName() const
    {
        return impl->GetName();
    }

    void GameObject::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    GameObject::Flags GameObject::GetFlags() const
    {
        return impl->GetFlags();
    }

    bool GameObject::IsDestroyed() const
    {
        return impl->IsDestroyed();
    }

    Resource<Transform> GameObject::GetTransform() const
    {
        return impl->GetTransform();
    }

    Resource<Component> GameObject::AddComponent(const std::type_info& type)
    {
        return impl->AddComponent(type);
    }

    Resource<Component> GameObject::GetComponent(const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponent(predicate);
    }

    std::vector<Resource<Component>> GameObject::GetComponents(
        const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponents(predicate);
    }

    Resource<Component> GameObject::GetComponentInChildren(
        const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponentInChildren(predicate);
    }

    std::vector<Resource<Component>> GameObject::GetComponentsInChildren(
        const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponentsInChildren(predicate);
    }

    void GameObject::Destroy()
    {
        impl->Destroy();
    }

    void GameObject::OnUpdate(const uint32_t currentFrame)
    {
        impl->OnUpdate(currentFrame);
    }
}
