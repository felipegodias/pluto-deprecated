#pragma once

#include "pluto/api.h"
#include "pluto/memory/resource_control.h"

#include <memory>

namespace pluto
{
    class Object;

    template <typename T>
    using IsObject = std::enable_if_t<std::is_base_of_v<Object, T>, int>;

    template <typename T, IsObject<T>  = 0>
    class PLUTO_API Resource
    {
        friend class MemoryManager;

        std::shared_ptr<ResourceControl> resourceControl;

        explicit Resource(const std::shared_ptr<ResourceControl> resourceControl)
            : resourceControl(resourceControl)
        {
        }

    public:
        T* Get() const
        {
            return dynamic_cast<const T*>(resourceControl->Get());
        }

        T* Get()
        {
            return dynamic_cast<T*>(resourceControl->Get());
        }

        T* operator->() const
        {
            return dynamic_cast<const T*>(resourceControl->Get());
        }

        T* operator->()
        {
            return dynamic_cast<T*>(resourceControl->Get());
        }
    };
}
