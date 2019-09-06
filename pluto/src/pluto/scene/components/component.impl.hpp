#pragma once

#include <utility>
#include "pluto/scene/components/component.h"
#include "pluto/scene/game_object.h"
#include "pluto/memory/resource.h"
#include "pluto/guid.h"

namespace pluto
{
    class Component::Impl
    {
        Guid guid;
        Resource<GameObject> gameObject;

    public:
        Impl(const Guid& guid, Resource<GameObject> gameObject)
            : guid(guid),
              gameObject(std::move(gameObject))
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return gameObject->GetName();
        }

        void SetName(const std::string& value)
        {
            gameObject->SetName(value);
        }

        Resource<GameObject> GetGameObject() const
        {
            return gameObject;
        }
    };
}
