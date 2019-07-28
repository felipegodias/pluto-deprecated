#include <pluto/scene/game_object.h>
#include <pluto/scene/components/component.h>
#include <pluto/scene/components/transform.h>
#include <pluto/scene/components/renderer.h>
#include <pluto/scene/components/mesh_renderer.h>
#include <pluto/scene/components/camera.h>

#include <pluto/service/service_collection.h>
#include <pluto/guid.h>
#include <pluto/exception.h>

#include <typeindex>
#include <unordered_map>
#include <iostream>

namespace pluto
{
    class GameObject::Impl
    {
        Guid guid;
        std::string name;
        Flags flags;

        Transform* transform;

        std::vector<std::unique_ptr<Component>> components;
        std::unordered_map<std::type_index, const Component::Factory*> componentFactories;

        uint32_t lastFrame;
        bool isDestroyed;
        GameObject* instance;

    public:
        Impl(const Guid& guid, const Transform::Factory& transformFactory, const Camera::Factory& cameraFactory,
             const MeshRenderer::Factory& meshRendererFactory)
            : guid(guid),
              flags(Flags::None),
              transform(nullptr),
              lastFrame(0),
              isDestroyed(false),
              instance(nullptr)
        {
            componentFactories.emplace(typeid(Transform), &transformFactory);
            componentFactories.emplace(typeid(Camera), &cameraFactory);
            componentFactories.emplace(typeid(MeshRenderer), &meshRendererFactory);
        }

        void Init(GameObject& instance)
        {
            this->instance = &instance;
            transform = dynamic_cast<Transform*>(&AddComponent(typeid(Transform)));
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(std::string value)
        {
            name = std::move(value);
        }

        Flags GetFlags() const
        {
            return flags;
        }

        bool IsDestroyed() const
        {
            return isDestroyed;
        }

        Transform& GetTransform() const
        {
            return *transform;
        }

        Component& AddComponent(const std::type_info& type)
        {
            const auto& factory = componentFactories.at(type);
            std::unique_ptr<Component> componentPtr = factory->Create(*instance);
            Component& component = *componentPtr;
            components.push_back(std::move(componentPtr));
            return component;
        }

        Component* GetComponent(const std::function<bool(const Component& component)>& predicate) const
        {
            for (auto& component : components)
            {
                if (predicate(*component))
                {
                    return component.get();
                }
            }
            return nullptr;
        }

        std::vector<std::reference_wrapper<Component>> GetComponents(
            const std::function<bool(const Component& component)>& predicate) const
        {
            std::vector<std::reference_wrapper<Component>> result;
            for (auto& component : components)
            {
                if (predicate(*component))
                {
                    result.emplace_back(*component);
                }
            }
            return result;
        }

        Component* GetComponentInChildren(const std::function<bool(const Component& component)>& predicate) const
        {
            Component* result = GetComponent(predicate);
            if (result != nullptr)
            {
                return result;
            }

            for (auto it : GetTransform().GetChildren())
            {
                Transform& child = it;
                result = child.GetGameObject().GetComponentInChildren(predicate);
                if (result != nullptr)
                {
                    return result;
                }
            }
            return nullptr;
        }

        std::vector<std::reference_wrapper<Component>> GetComponentsInChildren(
            const std::function<bool(const Component& component)>& predicate) const
        {
            std::vector<std::reference_wrapper<Component>> result = GetComponents(predicate);
            for (auto it : GetTransform().GetChildren())
            {
                Transform& child = it;
                auto componentsFromChild = child.GetGameObject().GetComponentsInChildren(predicate);
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
                Transform& child = it;
                child.GetGameObject().Destroy();
            }
        }

        void OnUpdate(const uint32_t currentFrame)
        {
            std::cout << name << std::endl;
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
                std::vector<std::reference_wrapper<Transform>> children = transform->GetChildren();
                for (auto& it : children)
                {
                    Transform& child = it;
                    child.GetGameObject().OnUpdate(currentFrame);
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
        auto& transformFactory = serviceCollection.GetService<Transform::Factory>();
        auto& cameraFactory = serviceCollection.GetService<Camera::Factory>();
        auto& meshRendererFactory = serviceCollection.GetService<MeshRenderer::Factory>();
        auto gameObject = std::make_unique<GameObject>(
            std::make_unique<Impl>(Guid::New(), transformFactory, cameraFactory, meshRendererFactory));
        gameObject->impl->Init(*gameObject);
        return gameObject;
    }

    GameObject::~GameObject() = default;

    GameObject::GameObject(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    GameObject::GameObject(GameObject&& other) noexcept = default;

    GameObject& GameObject::operator=(GameObject&& rhs) noexcept = default;

    bool GameObject::operator==(const GameObject& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    bool GameObject::operator!=(const GameObject& rhs) const
    {
        return !(*this == rhs);
    }

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

    Transform& GameObject::GetTransform() const
    {
        return impl->GetTransform();
    }

    Component& GameObject::AddComponent(const std::type_info& type)
    {
        return impl->AddComponent(type);
    }

    Component* GameObject::GetComponent(const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponent(predicate);
    }

    std::vector<std::reference_wrapper<Component>> GameObject::GetComponents(
        const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponents(predicate);
    }

    Component* GameObject::GetComponentInChildren(
        const std::function<bool(const Component& component)>& predicate) const
    {
        return impl->GetComponentInChildren(predicate);
    }

    std::vector<std::reference_wrapper<Component>> GameObject::GetComponentsInChildren(
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
