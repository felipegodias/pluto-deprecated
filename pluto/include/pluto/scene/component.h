#pragma once

#include "../api.h"

namespace pluto
{
    class Guid;
    class GameObject;

    class PLUTO_API Component
    {
    public:
        Component();
        Component(const Component& other) = delete;

        virtual ~Component() = 0;
        Component& operator=(const Component& rhs) = delete;

        bool operator==(const Component& rhs) const;
        bool operator!=(const Component& rhs) const;

        virtual const Guid& GetId() const = 0;
        virtual GameObject& GetGameObject() const = 0;

        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();
    };
}
