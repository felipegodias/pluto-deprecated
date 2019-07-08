#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>
#include <pluto/scene/component.h>
#include <pluto/di/di_container.h>
#include <pluto/guid.h>
#include <pluto/exception.h>

#include <typeindex>
#include <unordered_map>
#include <iostream>

namespace pluto
{
    class GameObject::Impl
    {
    private:
        Guid guid;
        std::string name;
        Flags flags;

        GameObject* me;
        Transform* transform;

        uint32_t lastFrame;
        bool isDestroyed;

        std::vector<std::unique_ptr<Component>> components;
        std::unordered_map<std::type_index, const BaseFactory&> componentFactories;

    public:
        Impl(Guid guid, const Transform::Factory& transformFactory) : guid(std::move(guid)),
                                                                      flags(Flags::None), me(nullptr),
                                                                      transform(nullptr),
                                                                      lastFrame(0), isDestroyed(false)
        {
            componentFactories.emplace(typeid(Transform), transformFactory);
        }

        void Init(GameObject& instance)
        {
            me = &instance;
            transform = &AddComponent<Transform>();
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

        template <typename T, IsComponent<T>  = 0>
        T& AddComponent()
        {
            const auto& factory = static_cast<const typename T::Factory&>(componentFactories.at(typeid(T)));
            std::unique_ptr<T> componentPtr = factory.Create(*me);
            T& component = *componentPtr;
            components.push_back(std::move(componentPtr));
            return component;
        }

        template <typename T, IsComponent<T>  = 0>
        T* GetComponent() const
        {
            for (auto& component : components)
            {
                T* result = dynamic_cast<T*>(component.get());
                if (result != nullptr)
                {
                    return result;
                }
            }
            return nullptr;
        }

        template <typename T, IsComponent<T>  = 0>
        std::vector<std::reference_wrapper<T>> GetComponents() const
        {
            std::vector<std::reference_wrapper<T>> result;
            for (auto& component : components)
            {
                T* obj = dynamic_cast<T*>(component.get());
                if (result != nullptr)
                {
                    result.push_back(*obj);
                }
            }
            return result;
        }

        template <typename T, IsComponent<T>  = 0>
        T* GetComponentInChildren() const
        {
            T* result = GetComponent<T>();
            if (result != nullptr)
            {
                return result;
            }

            for (auto& child : children)
            {
                result = child->GetComponentInChildren<T>();
                if (result != nullptr)
                {
                    return result;
                }
            }
            return nullptr;
        }

        template <typename T, IsComponent<T>  = 0>
        std::vector<std::reference_wrapper<T>> GetComponentsInChildren() const
        {
            std::vector<std::reference_wrapper<T>> result = GetComponents<T>();
            for (auto& child : children)
            {
                std::vector<std::reference_wrapper<T>> componentsFromChild = child->GetComponentInChildren<T>();
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

        void OnRender()
        {
            for (auto& component : components)
            {
                component->OnRender();
            }

            for (auto& it : transform->GetChildren())
            {
                Transform& child = it;
                child.GetGameObject().OnRender();
            }
        }
    };

    GameObject::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<GameObject> GameObject::Factory::Create() const
    {
        auto& transformFactory = diContainer.GetSingleton<Transform::Factory>();
        auto gameObject = std::make_unique<GameObject>(std::make_unique<Impl>(Guid::New(), transformFactory));
        gameObject->impl->Init(*gameObject);
        return gameObject;
    }

    GameObject::GameObject(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    GameObject::GameObject(GameObject&& other) noexcept : impl(std::move(other.impl))
    {
    }

    GameObject::~GameObject() = default;

    GameObject& GameObject::operator=(GameObject&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

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

    void GameObject::SetName(std::string value)
    {
        impl->SetName(std::move(value));
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

    template <typename T, IsComponent<T>>
    T& GameObject::AddComponent()
    {
        return impl->AddComponent<T>();
    }

    template <typename T, IsComponent<T>>
    T* GameObject::GetComponent() const
    {
        return impl->GetComponent<T>();
    }

    template <typename T, IsComponent<T>>
    std::vector<std::reference_wrapper<T>> GameObject::GetComponents() const
    {
        return impl->GetComponents<T>();
    }

    template <typename T, IsComponent<T>>
    T* GameObject::GetComponentInChildren() const
    {
        return impl->GetComponentInChildren<T>();
    }

    template <typename T, IsComponent<T>>
    std::vector<std::reference_wrapper<T>> GameObject::GetComponentsInChildren() const
    {
        return impl->GetComponentsInChildren<T>();
    }

    void GameObject::Destroy()
    {
        impl->Destroy();
    }

    void GameObject::OnUpdate(const uint32_t currentFrame)
    {
        impl->OnUpdate(currentFrame);
    }

    void GameObject::OnRender()
    {
        impl->OnRender();
    }
}
