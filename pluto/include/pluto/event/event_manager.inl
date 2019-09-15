#pragma once

#include "pluto/guid.h"

namespace pluto
{
    template <typename T, std::enable_if_t<std::is_base_of_v<BaseEvent, T>, bool>>
    Guid EventManager::Subscribe(const EventListener<T>& listener)
    {
        const auto callback = [listener](const BaseEvent& x)
        {
            listener(dynamic_cast<const T&>(x));
        };
        return Subscribe(typeid(T), callback);
    }

    template <typename T1, typename T2, std::enable_if_t<std::is_base_of_v<BaseEvent, T2>, bool>>
    Guid EventManager::Subscribe(T1& obj, EventMemberListener<T1, T2>&& listener)
    {
        return Subscribe<T2>(std::bind(listener, &obj, std::placeholders::_1));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseEvent, T>, bool>>
    void EventManager::Unsubscribe(const Guid& guid)
    {
        Unsubscribe(typeid(T), guid);
    }

    template <typename T,
              typename ... Args,
              std::enable_if_t<std::is_base_of_v<BaseEvent, T>
                               && std::is_constructible_v<T, Args...>, bool>>
    void EventManager::Dispatch(Args&& ... args) const
    {
        Dispatch(T(std::forward<Args>(args)...));
    }
}
