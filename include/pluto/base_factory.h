#pragma once

#include "singleton.h"

namespace pluto
{
    class DiContainer;

    class PLUTO_API BaseFactory : public ISingleton
    {
    protected:
        DiContainer& diContainer;

    public:
        explicit BaseFactory(DiContainer& diContainer);
        virtual ~BaseFactory() = 0;
    };
}
