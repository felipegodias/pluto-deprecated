#pragma once

#include "../api.h"

namespace pluto
{
    class Guid;
    class GameObject;

    class PLUTO_API Component
    {
    public:
        Component(const Component& other) = delete;

        virtual ~Component() = 0;
        Component& operator=(const Component& rhs) = delete;

        virtual const Guid& GetId() const = 0;
        virtual GameObject& GetGameObject() const = 0;

        virtual void Destroy() = 0;

        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;
    };
}
