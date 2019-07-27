#pragma once

#include "../api.h"
#include "base_service.h"
#include <memory>
#include <type_traits>
#include <typeindex>

namespace pluto
{
    template <typename T>
    using IsService = std::enable_if_t<std::is_base_of_v<BaseService, T>, int>;

    class PLUTO_API ServiceCollection
    {
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~ServiceCollection();
        explicit ServiceCollection();

        ServiceCollection(const ServiceCollection& other) = delete;
        ServiceCollection(ServiceCollection&& other) noexcept;
        ServiceCollection& operator=(const ServiceCollection& rhs) = delete;
        ServiceCollection& operator=(ServiceCollection&& rhs) noexcept;

        template <typename T, IsService<T>  = 0>
        T& AddService(std::unique_ptr<T> instance);

        BaseService& AddService(const std::type_index& type, std::unique_ptr<BaseService> instance);

        template <typename T, IsService<T>  = 0>
        void RemoveService();

        void RemoveService(const std::type_index& type);

        template <typename T, IsService<T>  = 0>
        T& GetService() const;

        BaseService& GetService(const std::type_index& type) const;
    };

    template <typename T, IsService<T>>
    T& ServiceCollection::AddService(std::unique_ptr<T> instance)
    {
        return static_cast<T&>(AddService(typeid(T), std::move(instance)));
    }

    template <typename T, IsService<T>>
    void ServiceCollection::RemoveService()
    {
        RemoveService(typeid(T));
    }

    template <typename T, IsService<T>>
    T& ServiceCollection::GetService() const
    {
        return static_cast<T&>(GetService(typeid(T)));
    }
}
