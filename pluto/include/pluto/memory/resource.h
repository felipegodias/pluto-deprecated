#pragma once

#include "pluto/api.h"
#include "pluto/memory/resource_control.h"

#include <memory>

namespace pluto
{
    template <typename T, typename Enable = void>
    class PLUTO_API Resource;

    template <typename T>
    class Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>
    {
        friend class MemoryManager;
        friend class ResourceUtils;

        std::shared_ptr<ResourceControl> control;

        explicit Resource(std::unique_ptr<ResourceControl> control);

    public:
        Resource();
        Resource(nullptr_t);

        template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>  = false>
        Resource(const Resource<T2>& other);
        template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>  = false>
        Resource(Resource<T2>&& other) noexcept;
        template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>  = false>
        Resource& operator=(const Resource<T2>& rhs);
        template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>  = false>
        Resource& operator=(Resource<T2>&& rhs);

        bool operator==(nullptr_t) const;
        bool operator!=(nullptr_t) const;

        const T* Get() const;
        T* Get();

        const T* operator->() const;
        T* operator->();
    };

    class PLUTO_API ResourceUtils
    {
    public:
        template <typename T1,
                  typename T2,
                  std::enable_if_t<std::is_base_of_v<Object, T2>, bool>  = false,
                  std::enable_if_t<std::is_base_of_v<T2, T1>, bool>  = false>
        static Resource<T1> Cast(const Resource<T2>& resource);
    };

    template <typename T>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Resource(std::unique_ptr<ResourceControl> control)
        : control(control.release())
    {
    }

    template <typename T>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Resource() = default;

    template <typename T>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Resource(nullptr_t)
    {
    }

    template <typename T>
    template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Resource(const Resource<T2>& other)
        : control(other.control)
    {
    }

    template <typename T>
    template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Resource(Resource<T2>&& other) noexcept
        : control(std::move(other.control))
    {
    }

    template <typename T>
    template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>& Resource<
        T, std::enable_if_t<std::is_base_of_v<Object, T>>>::operator=(const Resource<T2>& rhs)
    {
        control = rhs.control;
        return *this;
    }

    template <typename T>
    template <typename T2, std::enable_if_t<std::is_base_of_v<T, T2>, bool>>
    Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>& Resource<
        T, std::enable_if_t<std::is_base_of_v<Object, T>>>::operator=(Resource<T2>&& rhs)
    {
        control = std::move(rhs.control);
        return *this;
    }

    template <typename T>
    bool Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::operator==(nullptr_t) const
    {
        return !control || control->Get() == nullptr;
    }

    template <typename T>
    bool Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::operator!=(nullptr_t) const
    {
        return !(*this == nullptr);
    }

    template <typename T>
    const T* Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Get() const
    {
        if (control == nullptr)
        {
            return nullptr;
        }
        return dynamic_cast<const T*>(control->Get());
    }

    template <typename T>
    T* Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::Get()
    {
        if (control == nullptr)
        {
            return nullptr;
        }
        return dynamic_cast<T*>(control->Get());
    }

    template <typename T>
    const T* Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::operator->() const
    {
        return Get();
    }

    template <typename T>
    T* Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>::operator->()
    {
        return Get();
    }

    template <typename T1,
              typename T2,
              std::enable_if_t<std::is_base_of_v<Object, T2>, bool>,
              std::enable_if_t<std::is_base_of_v<T2, T1>, bool>>
    Resource<T1> ResourceUtils::Cast(const Resource<T2>& resource)
    {
        if (dynamic_cast<const T1*>(resource.Get()) == nullptr)
        {
            return nullptr;
        }
        Resource<T1> out;
        out.control = resource.control;
        return out;
    }
}
