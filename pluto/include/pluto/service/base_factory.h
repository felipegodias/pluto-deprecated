#pragma once

#include "base_service.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API BaseFactory : public BaseService
    {
        ServiceCollection* diContainer;

    public:
        virtual ~BaseFactory() = 0;
        explicit BaseFactory(ServiceCollection& diContainer);

        ServiceCollection& GetServiceCollection() const;
    };
}
