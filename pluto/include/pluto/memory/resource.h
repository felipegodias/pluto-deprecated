#pragma once

#include "pluto/api.h"
#include "pluto/memory/resource_control.h"

#include <memory>

namespace pluto
{
    class Object;

    template <typename T1, typename T2>
    using IsBaseOf = std::enable_if_t<std::is_base_of_v<T1, T2>, bool>;

    template <typename T>
    using IsObject = std::enable_if_t<std::is_base_of_v<Object, T>, bool>;

    template <typename T, typename Enable = void>
    class PLUTO_API Resource;

    template <typename T>
    class Resource<T, std::enable_if_t<std::is_base_of_v<Object, T>>>
    {
        friend class MemoryManager;
        friend class ResourceUtils;

        std::shared_ptr<ResourceControl> control;

        explicit Resource(std::unique_ptr<ResourceControl> control)
            : control(control.release())
        {
        }

    public:
        Resource()
        {
        }

        Resource(nullptr_t)
        {
        }

        template <typename T2, IsBaseOf<T, T2>  = 0>
        Resource(const Resource<T2>& other)
            : control(other.control)
        {
        }

        template <typename T2, IsBaseOf<T, T2>  = 0>
        Resource(Resource<T2>&& other) noexcept
            : control(std::move(other.control))
        {
        }

        template <typename T2, IsBaseOf<T, T2>  = 0>
        Resource& operator=(const Resource<T2>& rhs)
        {
            control = rhs.control;
            return *this;
        }

        template <typename T2, IsBaseOf<T, T2>  = 0>
        Resource& operator=(Resource<T2>&& rhs)
        {
            control = std::move(rhs.control);
            return *this;
        }

        bool operator==(nullptr_t) const
        {
            return control == nullptr || control->Get() == nullptr;
        }

        bool operator!=(nullptr_t) const
        {
            return !(*this == nullptr);
        }

        const T* Get() const
        {
            if (control == nullptr)
            {
                return nullptr;
            }
            return dynamic_cast<const T*>(control->Get());
        }

        T* Get()
        {
            if (control == nullptr)
            {
                return nullptr;
            }
            return dynamic_cast<T*>(control->Get());
        }

        const T* operator->() const
        {
            if (control == nullptr)
            {
                return nullptr;
            }
            return dynamic_cast<const T*>(control->Get());
        }

        T* operator->()
        {
            if (control == nullptr)
            {
                return nullptr;
            }
            return dynamic_cast<T*>(control->Get());
        }
    };

    class ResourceUtils
    {
    public:
        template <typename T1, typename T2, IsObject<T2>  = 0, IsBaseOf<T2, T1>  = 0>
        static Resource<T1> Cast(const Resource<T2>& resource)
        {
            if (dynamic_cast<const T1*>(resource.Get()) == nullptr)
            {
                return nullptr;
            }
            Resource<T1> out;
            out.control = resource.control;
            return out;
        }
    };
}
