#pragma once

#include "pluto/api.h"
#include <memory>
#include <vector>
#include <functional>

namespace pluto
{
    class BaseService;
    class BaseFactory;

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
        template <typename T, typename ... Args, std::enable_if_t<
                      std::is_base_of_v<BaseService, T> && std::is_constructible_v<T, Args...>, bool>  = false>
        T& EmplaceService(Args&& ... args);
        template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>  = false>
        void RemoveService();
        template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>  = false>
        T& GetService() const;

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseFactory, typename T::Factory>, bool>  = false>
        typename T::Factory& AddFactory(std::unique_ptr<typename T::Factory> instance);

        template <typename T, typename ... Args, std::enable_if_t<
                      std::is_constructible_v<typename T::Factory, ServiceCollection&, Args...>, bool>  = false>
        typename T::Factory& EmplaceFactory(Args&& ... args);

        template <typename T, typename F, typename ... Args, std::enable_if_t<
                      std::is_base_of_v<typename T::Factory, F> && std::is_constructible_v<
                          F, ServiceCollection&, Args...>, bool>  = false>
        F& EmplaceFactory(Args&& ... args);

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseFactory, typename T::Factory>, bool>  = false>
        void RemoveFactory();
        template <typename T, std::enable_if_t<std::is_base_of_v<BaseFactory, typename T::Factory>, bool>  = false>
        typename T::Factory& GetFactory() const;

        BaseService& AddService(const std::type_info& type, std::unique_ptr<BaseService> instance);
        void RemoveService(const std::type_info& type);
        BaseService& GetService(const std::type_info& type) const;

        std::vector<std::reference_wrapper<BaseService>> FindServices(
            const std::function<bool(const BaseService& baseService)>& predicate) const;

        BaseFactory& AddFactory(const std::type_info& type, std::unique_ptr<BaseFactory> instance);
        void RemoveFactory(const std::type_info& type);
        BaseFactory& GetFactory(const std::type_info& type) const;
    };
}

#include "service_collection.inl"
