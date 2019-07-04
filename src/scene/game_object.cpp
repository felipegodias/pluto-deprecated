#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>
#include <pluto/scene/component.h>
#include <pluto/guid.h>
#include <pluto/di/di_container.h>

#include <typeindex>
#include <unordered_map>

namespace pluto
{
    class GameObject::Impl
    {
    private:
        Guid guid;
        std::string name;
        Flags flags;

        GameObject* me;
        GameObject* parent;
        Transform* transform;

        uint32_t lastFrame;
        bool isDestroyed;

        std::vector<std::unique_ptr<GameObject>> children;
        std::vector<std::unique_ptr<Component>> components;
        std::unordered_map<std::type_index, const BaseFactory&> componentFactories;

    public:
        Impl(Guid guid, const Transform::Factory& transformFactory) : guid(std::move(guid)),
                                                                      flags(Flags::None), me(nullptr),
                                                                      parent(nullptr), transform(nullptr),
                                                                      isDestroyed(false)
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

        Transform& GameObject::GetTransform() const
        {
            return *transform;
        }

        GameObject& GameObject::GetParent() const
        {
            return *parent;
        }

        void GameObject::SetParent(GameObject& value)
        {
            if (flags == Flags::Static || isDestroyed || value.impl->isDestroyed)
            {
                return;
            }

            std::unique_ptr<GameObject> myPtr = parent->impl->RemoveAndGetChildPtr(*me);
            parent = &value;
            parent->impl->children.push_back(std::move(myPtr));
        }

        uint32_t GetChildCount() const
        {
            return children.size();
        }

        GameObject& GameObject::GetChild(const int index)
        {
            return *children[index];
        }

        std::vector<GameObject&> GameObject::GetChildren() const
        {
            std::vector<GameObject&> result;
            for (int i = 0; i < children.size(); ++i)
            {
                GameObject& child = *children[i];
                result.emplace_back(child);
            }
            return result;
        }

        template <typename T, IsComponent<T>  = 0>
        T& GameObject::AddComponent()
        {
            const auto& factory = static_cast<const typename T::Factory&>(componentFactories.at(typeid(T)));
            std::unique_ptr<T> componentPtr = factory.Create(*me);
            T& component = *componentPtr;
            components.push_back(std::move(componentPtr));
            return component;
        }

        template <typename T, IsComponent<T>  = 0>
        T* GameObject::GetComponent() const
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
        std::vector<T&> GameObject::GetComponents() const
        {
            std::vector<T&> result;
            for (auto& component : components)
            {
                T* obj = dynamic_cast<T*>(component.get());
                if (result != nullptr)
                {
                    result.emplace_back(*obj);
                }
            }
            return result;
        }

        template <typename T, IsComponent<T>  = 0>
        T* GameObject::GetComponentInChildren() const
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
        std::vector<T&> GameObject::GetComponentsInChildren() const
        {
            std::vector<T&> result = GetComponents<T>();
            for (auto& child : children)
            {
                std::vector<T&> componentsFromChild = child->GetComponentInChildren<T>();
                result.insert(componentsFromChild.begin(), componentsFromChild.end());
            }
            return result;
        }

        void GameObject::Destroy()
        {
            if (isDestroyed)
            {
                return;
            }

            isDestroyed = true;

            for (auto& component : components)
            {
                component->Destroy();
            }

            for (auto& child : children)
            {
                child->Destroy();
            }
        }

        void GameObject::OnUpdate(const uint32_t currentFrame)
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
                for (auto& child : GetChildren())
                {
                    child.OnUpdate(currentFrame);
                }
            }

            std::unique_ptr<GameObject> myPtr = nullptr;
            // Clear from memory.
            if (isDestroyed && parent != nullptr && !parent->impl->isDestroyed)
            {
                myPtr = parent->impl->RemoveAndGetChildPtr(*me);
            }
            // myPtr scoped out and die.
        }

        void GameObject::OnRender()
        {
            for (auto& component : components)
            {
                component->OnRender();
            }

            for (auto& child : children)
            {
                child->OnRender();
            }
        }

    private:
        std::unique_ptr<GameObject> RemoveAndGetChildPtr(const GameObject& gameObject)
        {
            auto it = children.begin();
            while (it != children.end())
            {
                if (**it != gameObject)
                {
                    ++it;
                    continue;
                }

                std::unique_ptr<GameObject> child = std::move(*it);
                children.erase(it);
                return child;
            }
            return nullptr;
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

    Transform& GameObject::GetTransform() const
    {
        return impl->GetTransform();
    }

    GameObject& GameObject::GetParent() const
    {
        return impl->GetParent();
    }

    void GameObject::SetParent(GameObject& value)
    {
        impl->SetParent(value);
    }

    uint32_t GameObject::GetChildCount() const
    {
        return impl->GetChildCount();
    }

    GameObject& GameObject::GetChild(const int index)
    {
        return impl->GetChild(index);
    }

    std::vector<GameObject&> GameObject::GetChildren() const
    {
        return impl->GetChildren();
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
    std::vector<T&> GameObject::GetComponents() const
    {
        return impl->GetComponents<T>();
    }

    template <typename T, IsComponent<T>>
    T* GameObject::GetComponentInChildren() const
    {
        return impl->GetComponentInChildren<T>();
    }

    template <typename T, IsComponent<T>>
    std::vector<T&> GameObject::GetComponentsInChildren() const
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
