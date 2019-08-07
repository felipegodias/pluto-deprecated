#include <pluto/service/service_collection.h>
#include <pluto/service/base_service.h>
#include <pluto/service/base_factory.h>

#include <pluto/log/log_manager.h>

#include "pluto/exception.h"

#include <fmt/format.h>

#include <unordered_map>
#include <typeindex>

namespace pluto
{
    class ServiceCollection::Impl
    {
        std::unordered_map<std::type_index, std::unique_ptr<BaseService>> services;
        std::unordered_map<std::type_index, std::unique_ptr<BaseFactory>> factories;

    public:
        BaseService& AddService(const std::type_info& type, std::unique_ptr<BaseService> instance)
        {
            const auto it = services.find(type);
            if (it != services.end())
            {
                Exception::Throw(
                    std::runtime_error(fmt::format(
                        "Service instance of type {0} already exists in the service collection.",
                        type.name())));
            }

            BaseService* ptr = instance.get();
            services.emplace(type, std::move(instance));
            return *ptr;
        }

        void RemoveService(const std::type_info& type)
        {
            services.erase(type);
        }

        BaseService& GetService(const std::type_info& type) const
        {
            const auto it = services.find(type);
            if (it == services.end())
            {
                Exception::Throw(
                    std::runtime_error(fmt::format("Could not resolve service with type {0}.", type.name())));
            }

            return *it->second;
        }

        BaseFactory& AddFactory(const std::type_info& type, std::unique_ptr<BaseFactory> instance)
        {
            const auto it = factories.find(type);
            if (it != factories.end())
            {
                Exception::Throw(
                    std::runtime_error(fmt::format(
                        "Factory for type {0} already exists in the service collection.",
                        type.name())));
            }

            BaseFactory* ptr = instance.get();
            factories.emplace(type, std::move(instance));
            return *ptr;
        }

        void RemoveFactory(const std::type_info& type)
        {
            factories.erase(type);
        }

        BaseFactory& GetFactory(const std::type_info& type) const
        {
            const auto it = factories.find(type);
            if (it == factories.end())
            {
                Exception::Throw(
                    std::runtime_error(fmt::format("Could not resolve factory for type {0}.", type.name())));
            }

            return *it->second;
        }
    };

    ServiceCollection::~ServiceCollection() = default;

    ServiceCollection::ServiceCollection()
        : impl(std::make_unique<Impl>())
    {
    }

    ServiceCollection::ServiceCollection(ServiceCollection&& other) noexcept = default;

    ServiceCollection& ServiceCollection::operator=(ServiceCollection&& rhs) noexcept = default;

    BaseService& ServiceCollection::AddService(const std::type_info& type, std::unique_ptr<BaseService> instance)
    {
        return impl->AddService(type, std::move(instance));
    }

    void ServiceCollection::RemoveService(const std::type_info& type)
    {
        impl->RemoveService(type);
    }

    BaseService& ServiceCollection::GetService(const std::type_info& type) const
    {
        return impl->GetService(type);
    }

    BaseFactory& ServiceCollection::AddFactory(const std::type_info& type, std::unique_ptr<BaseFactory> instance)
    {
        return impl->AddFactory(type, std::move(instance));
    }

    void ServiceCollection::RemoveFactory(const std::type_info& type)
    {
        impl->RemoveFactory(type);
    }

    BaseFactory& ServiceCollection::GetFactory(const std::type_info& type) const
    {
        return impl->GetFactory(type);
    }
}
