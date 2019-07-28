#pragma once

#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Guid;
    class GameObject;

    class PLUTO_API Component
    {
    public:
        class Factory : public BaseFactory
        {
        public:
            ~Factory() override;
            explicit Factory(ServiceCollection& diContainer);

            Factory(const Factory& other) = delete;
            Factory(Factory&& other) noexcept;
            Factory& operator=(const Factory& rhs) = delete;
            Factory& operator=(Factory&& rhs) noexcept;

            virtual std::unique_ptr<Component> Create(GameObject& gameObject) const = 0;
        };

        virtual ~Component() = 0;
        Component();

        Component(const Component& other) = delete;
        Component(Component&& other) noexcept;
        Component& operator=(const Component& rhs) = delete;
        Component& operator=(Component&& rhs) noexcept;

        bool operator==(const Component& rhs) const;
        bool operator!=(const Component& rhs) const;

        virtual const Guid& GetId() const = 0;
        virtual GameObject& GetGameObject() const = 0;

        virtual void OnUpdate();
        virtual void OnDestroy();
    };
}
