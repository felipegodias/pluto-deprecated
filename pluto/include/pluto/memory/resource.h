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
        bool operator==(const Resource& rhs) const;
        bool operator!=(const Resource& rhs) const;

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
}

#include "resource.inl"
