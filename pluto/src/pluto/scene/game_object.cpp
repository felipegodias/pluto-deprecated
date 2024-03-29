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
        bool isActive;
        Flags flags;

        Resource<Transform> transform;

        std::vector<Resource<Component>> components;
        bool isDestroyed;

        MemoryManager* memoryManager;
        ServiceCollection* serviceCollection;

    public:
        Impl(const Guid& guid, MemoryManager& memoryManager, ServiceCollection& serviceCollection)
            : guid(guid),
              isActive(true),
              flags(Flags::None),
              transform(nullptr),
              isDestroyed(false),
              memoryManager(&memoryManager),
              serviceCollection(&serviceCollection)
        {
        }

        ~Impl()
        {
            for (auto& component : components)
            {
                memoryManager->Remove(*component.Get());
            }
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

        bool IsGloballyActive() const
        {
            if (transform->IsRoot())
            {
                return isActive;
            }

            return isActive && transform->GetParent()->GetGameObject()->IsGloballyActive();
        }

        bool IsActive() const
        {
            return isActive;
        }

        void SetActive(const bool value)
        {
            isActive = value;
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
            if (!IsGloballyActive())
            {
                return nullptr;
            }

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
            std::vector<Resource<Component>> result;
            if (!IsGloballyActive())
            {
                return result;
            }

            result = GetComponents(predicate);
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

        void OnEarlyFixedUpdate()
        {
            EvaluateComponents(&Component::OnEarlyFixedUpdate);
        }

        void OnFixedUpdate()
        {
            EvaluateComponents(&Component::OnFixedUpdate);
        }

        void OnLateFixedUpdate()
        {
            EvaluateComponents(&Component::OnLateFixedUpdate);
        }

        void OnCollision2DBegin(const Collision2D& collision)
        {
            EvaluateComponents(&Component::OnCollision2DBegin, collision);
        }

        void OnCollision2DEnd(const Collision2D& collision)
        {
            EvaluateComponents(&Component::OnCollision2DEnd, collision);
        }

        void OnTrigger2DEnter(const Resource<Collider2D>& collider)
        {
            EvaluateComponents(&Component::OnTrigger2DEnter, collider);
        }

        void OnTrigger2DExit(const Resource<Collider2D>& collider)
        {
            EvaluateComponents(&Component::OnTrigger2DExit, collider);
        }

        void OnEarlyUpdate()
        {
            EvaluateComponents(&Component::OnEarlyUpdate);
        }

        void OnUpdate()
        {
            EvaluateComponents(&Component::OnUpdate);
        }

        void OnLateUpdate()
        {
            EvaluateComponents(&Component::OnLateUpdate);
        }

        void OnPreRender()
        {
            EvaluateComponents(&Component::OnPreRender);
        }

        void OnRender()
        {
            EvaluateComponents(&Component::OnRender);
        }

        void OnPostRender()
        {
            EvaluateComponents(&Component::OnPostRender);
        }

    private:
        template <typename ... Args>
        using ComponentFunction = void(Component::*)(Args ...);

        template <typename ... Args>
        void EvaluateComponents(ComponentFunction<Args...>&& function, Args&& ... args)
        {
            if (!IsGloballyActive() || isDestroyed)
            {
                return;
            }

            for (auto& component : components)
            {
                Component* ptr = component.Get();
                (*ptr.*function)(std::forward<Args>(args)...);
            }
        }
    };

    GameObject::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
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

    bool GameObject::IsGloballyActive() const
    {
        return impl->IsGloballyActive();
    }

    bool GameObject::IsActive() const
    {
        return impl->IsActive();
    }

    void GameObject::SetActive(const bool value)
    {
        impl->SetActive(value);
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

    void GameObject::OnEarlyFixedUpdate()
    {
        impl->OnEarlyFixedUpdate();
    }

    void GameObject::OnFixedUpdate()
    {
        impl->OnFixedUpdate();
    }

    void GameObject::OnLateFixedUpdate()
    {
        impl->OnLateFixedUpdate();
    }

    void GameObject::OnCollision2DBegin(const Collision2D& collision)
    {
        impl->OnCollision2DBegin(collision);
    }

    void GameObject::OnCollision2DEnd(const Collision2D& collision)
    {
        impl->OnCollision2DEnd(collision);
    }

    void GameObject::OnTrigger2DEnter(const Resource<Collider2D>& collider)
    {
        impl->OnTrigger2DEnter(collider);
    }

    void GameObject::OnTrigger2DExit(const Resource<Collider2D>& collider)
    {
        impl->OnTrigger2DExit(collider);
    }

    void GameObject::OnEarlyUpdate()
    {
        impl->OnEarlyUpdate();
    }

    void GameObject::OnUpdate()
    {
        impl->OnUpdate();
    }

    void GameObject::OnLateUpdate()
    {
        impl->OnLateUpdate();
    }

    void GameObject::OnPreRender()
    {
        impl->OnPreRender();
    }

    void GameObject::OnRender()
    {
        impl->OnRender();
    }

    void GameObject::OnPostRender()
    {
        impl->OnPostRender();
    }
}
