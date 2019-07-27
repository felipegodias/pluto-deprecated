#pragma once

#include "singleton.h"

namespace pluto
{
    class DiContainer;

    class PLUTO_API BaseFactory : public Singleton
    {
        DiContainer* diContainer;

    public:
        virtual ~BaseFactory() = 0;
        explicit BaseFactory(DiContainer& diContainer);

        DiContainer& GetServiceCollection() const;
    };
}
