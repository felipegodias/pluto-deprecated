#pragma once

#include "../api.h"
#include "base_service.h"
#include <memory>
#include <type_traits>

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

        template <typename T, IsService<T>  = 0>
        T& AddSingleton(std::unique_ptr<T> instance);
        template <typename T, IsService<T>  = 0>
        void RemoveSingleton();
        template <typename T, IsService<T>  = 0>
        T& GetSingleton() const;
    };
}
