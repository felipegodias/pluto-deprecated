#pragma once

#include "../api.h"
#include "base_service.h"
#include <memory>
#include <type_traits>
#include <typeindex>

namespace pluto
{
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

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>  = false>
        T& AddService(std::unique_ptr<T> instance);

        BaseService& AddService(const std::type_index& type, std::unique_ptr<BaseService> instance);

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>  = false>
        void RemoveService();

        void RemoveService(const std::type_index& type);

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>  = false>
        T& GetService() const;

        BaseService& GetService(const std::type_index& type) const;
    };
}

#include "service_collection.inl"
