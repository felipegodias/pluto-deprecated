#pragma once

#include "base_service.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API BaseFactory
    {
        ServiceCollection* serviceCollection;

    public:
        virtual ~BaseFactory() = 0;
        explicit BaseFactory(ServiceCollection& serviceCollection);

        BaseFactory(const BaseFactory& other) = delete;
        BaseFactory(BaseFactory&& other) noexcept;
        BaseFactory& operator=(const BaseFactory& rhs) = delete;
        BaseFactory& operator=(BaseFactory&& rhs) noexcept;

        ServiceCollection& GetServiceCollection() const;
    };
}
