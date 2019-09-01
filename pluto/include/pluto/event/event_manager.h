#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"
#include "base_event.h"

#include <functional>
#include <memory>
#include <type_traits>

namespace pluto
{
    class Guid;

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseEvent, T>, bool>  = false>
    using EventListener = std::function<void(const T&)>;

    class PLUTO_API EventManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<EventManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~EventManager();
        explicit EventManager(std::unique_ptr<Impl> impl);

        EventManager(const EventManager& other) = delete;
        EventManager(EventManager&& other) noexcept;
        EventManager& operator=(const EventManager& other) = delete;
        EventManager& operator=(EventManager&& other) noexcept;

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseEvent, T>, bool>  = false>
        Guid Subscribe(const EventListener<T>& listener);

        template <typename T, std::enable_if_t<std::is_base_of_v<BaseEvent, T>, bool>  = false>
        void Unsubscribe(const Guid& guid);

        template <typename T,
                  typename ... Args,
                  std::enable_if_t<std::is_base_of_v<BaseEvent, T>
                                   && std::is_constructible_v<T, Args...>, bool>  = false>
        void Dispatch(Args&&... args) const;

        Guid Subscribe(const std::type_info& eventType, const EventListener<BaseEvent>& listener);

        void Unsubscribe(const std::type_info& eventType, const Guid& guid);

        void Dispatch(const BaseEvent& event) const;
    };
}

#include "event_manager.inl"
