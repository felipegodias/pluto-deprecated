#pragma once

#include "pluto/api.h"
#include "pluto/memory/resource_control.h"

#include <memory>

namespace pluto
{
    class Object;

    template <typename T1, typename T2>
    using CanCast = std::enable_if_t<std::is_base_of_v<T1, T2> || std::is_base_of_v<T2, T1>, int>;

    template <typename T>
    using IsObject = std::enable_if_t<std::is_base_of_v<Object, T>, int>;

    template <typename T, IsObject<T>  = 0>
    class PLUTO_API Resource
    {
        friend class MemoryManager;

        std::shared_ptr<ResourceControl> resourceControl;

        explicit Resource(std::unique_ptr<ResourceControl> resourceControl)
            : resourceControl(resourceControl.release())
        {
        }

    public:
        template <typename T2, IsObject<T2>  = 0, CanCast<T, T2>  = 0>
        explicit Resource(const Resource<T2>& other)
            : resourceControl(other.resourceControl)
        {
        }

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

    template <typename T1, typename T2, IsObject<T2>  = 0, CanCast<T1, T2>  = 0>
    std::shared_ptr<Resource<T1>> ResourceCast(const std::shared_ptr<Resource<T2>>& resource)
    {
        return std::make_shared<Resource<T1>>(*resource);
    }
}
