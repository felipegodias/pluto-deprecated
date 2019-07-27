#pragma once

#include "../api.h"
#include "singleton.h"
#include <memory>
#include <type_traits>

namespace pluto
{
    template <typename T>
    using IsSingleton = std::enable_if_t<std::is_base_of_v<Singleton, T>, int>;

    class PLUTO_API DiContainer
    {
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~DiContainer();
        explicit DiContainer();

        template <typename T, IsSingleton<T>  = 0>
        T& AddSingleton(std::unique_ptr<T> instance);
        template <typename T, IsSingleton<T>  = 0>
        void RemoveSingleton();
        template <typename T, IsSingleton<T>  = 0>
        T& GetSingleton() const;
    };
}
