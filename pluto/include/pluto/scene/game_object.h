#pragma once

#include "pluto/service/base_factory.h"

#include <memory>
#include <string>
#include <vector>

namespace pluto
{
    class Guid;
    class Transform;
    class Component;

    template <typename T>
    using IsComponent = std::enable_if_t<std::is_base_of_v<Component, T>, int>;

    class PLUTO_API GameObject
    {
    public:
        enum class Flags
        {
            None = 0,
            Static = 1,
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<GameObject> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit GameObject(std::unique_ptr<Impl> impl);

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) noexcept;
        ~GameObject();

        GameObject& operator=(const GameObject& rhs) = delete;
        GameObject& operator=(GameObject&& rhs) noexcept;

        bool operator==(const GameObject& rhs) const;
        bool operator!=(const GameObject& rhs) const;

        const Guid& GetId() const;
        const std::string& GetName() const;
        void SetName(std::string value);

        Flags GetFlags() const;
        bool IsDestroyed() const;

        Transform& GetTransform() const;

        template <typename T, IsComponent<T>  = 0>
        T& AddComponent();

        template <typename T, IsComponent<T>  = 0>
        T* GetComponent() const;

        template <typename T, IsComponent<T>  = 0>
        std::vector<std::reference_wrapper<T>> GetComponents() const;

        template <typename T, IsComponent<T>  = 0>
        T* GetComponentInChildren() const;

        template <typename T, IsComponent<T>  = 0>
        std::vector<std::reference_wrapper<T>> GetComponentsInChildren() const;

        void Destroy();

        void OnUpdate(uint32_t currentFrame);
    };
}
