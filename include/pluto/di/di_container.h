#pragma once

#include "../api.h"
#include <memory>

namespace pluto
{
    class Singleton;

    class PLUTO_API DiContainer
    {
    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit DiContainer();
        ~DiContainer();

        template <typename T>
        T& AddSingleton(std::unique_ptr<T> instance);
        template <typename T>
        void RemoveSingleton();
        template <typename T>
        T& Resolve() const;
    };
}
