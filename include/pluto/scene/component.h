#pragma once

#include "../api.h"

namespace pluto
{
    class Guid;
    class GameObject; 

    class PLUTO_API Component
    {
    public:
        virtual ~Component() = 0;
        virtual const Guid& GetId() const = 0;
        virtual GameObject& GetGameObject() const = 0;

        virtual void Destroy() = 0;

        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
    };
}
