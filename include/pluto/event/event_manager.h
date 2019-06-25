#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
#include "base_event.h"
#include <functional>
#include <memory>
#include <type_traits>

namespace pluto
{
    class Guid;

    template <typename T>
    using IsEvent = std::enable_if_t<std::is_base_of_v<BaseEvent, T>, int>;

    template <typename T, IsEvent<T>  = 0>
    using EventListener = std::function<void(const T&)>;

    class PLUTO_API EventManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<EventManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit EventManager(std::unique_ptr<Impl> impl);
        ~EventManager();

        template <typename T, IsEvent<T>  = 0>
        Guid Subscribe(const EventListener<T>& listener);

        template <typename T, IsEvent<T>  = 0>
        void Unsubscribe(const Guid& guid);

        template <typename T, IsEvent<T>  = 0>
        void Dispatch(const T& event) const;
    };
}
