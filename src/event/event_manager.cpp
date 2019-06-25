#include <pluto/event/event_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>

#include <pluto/guid.h>

#include <pluto/event/on_startup_event.h>
#include <pluto/simulation/on_pre_update_event.h>
#include <pluto/simulation/on_update_event.h>
#include <pluto/simulation/on_post_update_event.h>

#include <memory>
#include <typeindex>
#include <vector>
#include <unordered_map>

namespace pluto
{
    class EventManager::Impl
    {
        struct EventListenerData final
        {
            Guid guid;
            EventListener<BaseEvent> callback;
        };

        LogManager& logManager;
        std::unordered_map<std::type_index, std::vector<EventListenerData>> listeners;

    public:
        explicit Impl(LogManager& logManager) : logManager(logManager)
        {
            logManager.LogInfo("EventManager Initialized!");
        }

        ~Impl()
        {
            logManager.LogInfo("EventManager Terminated!");
        }

        template <typename T, IsEvent<T>  = 0>
        Guid Subscribe(const EventListener<T>& listener)
        {
            const std::type_index type = typeid(T);
            const auto it = listeners.find(type);
            if (it == listeners.end())
            {
                listeners[type] = std::vector<EventListenerData>();
            }
            const auto callback = [listener](const BaseEvent& x)
            {
                listener(static_cast<const T&>(x));
            };
            auto& vec = listeners[type];
            Guid guid = Guid::New();
            vec.push_back({ guid, callback });
            return guid;
        }

        template <typename T, IsEvent<T>  = 0>
        void Unsubscribe(const Guid& guid)
        {
            const auto it = listeners.find(typeid(T));
            if (it == listeners.end())
            {
                return;
            }

            // TODO: Improve here, this can perform badly when a given event is listen in lot of places. e.g. OnUpdateEvent.
            for (auto i = it->second.begin(); i != it->second.end(); ++i)
            {
                if (i->guid == guid)
                {
                    it->second.erase(i);
                    break;
                }
            }
            if (it->second.empty())
            {
                listeners.erase(it);
            }
        }

        template <typename T, IsEvent<T>  = 0>
        void Dispatch(const T& event) const
        {
            const auto it = listeners.find(typeid(T));
            if (it == listeners.end())
            {
                return;
            }
            for (auto& listener : it->second)
            {
                listener.callback(event);
            }
        }
    };

    EventManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<EventManager> EventManager::Factory::Create() const
    {
        auto& logManager = diContainer.GetSingleton<LogManager>();
        return std::make_unique<EventManager>(std::make_unique<Impl>(logManager));
    }

    EventManager::EventManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    EventManager::~EventManager() = default;

    template <typename T, IsEvent<T>>
    Guid EventManager::Subscribe(const EventListener<T>& listener)
    {
        return impl->Subscribe(listener);
    }

    template <typename T, IsEvent<T>>
    void EventManager::Unsubscribe(const Guid& guid)
    {
        impl->Unsubscribe<T>(guid);
    }

    template <typename T, IsEvent<T>>
    void EventManager::Dispatch(const T& event) const
    {
        impl->Dispatch(event);
    }

    template Guid EventManager::Subscribe(const EventListener<OnStartupEvent>& listener);
    template void EventManager::Unsubscribe<OnStartupEvent>(const Guid& guid);
    template void EventManager::Dispatch(const OnStartupEvent& event) const;

    template Guid EventManager::Subscribe(const EventListener<OnPreUpdateEvent>& listener);
    template void EventManager::Unsubscribe<OnPreUpdateEvent>(const Guid& guid);
    template void EventManager::Dispatch(const OnPreUpdateEvent& event) const;

    template Guid EventManager::Subscribe(const EventListener<OnUpdateEvent>& listener);
    template void EventManager::Unsubscribe<OnUpdateEvent>(const Guid& guid);
    template void EventManager::Dispatch(const OnUpdateEvent& event) const;

    template Guid EventManager::Subscribe(const EventListener<OnPostUpdateEvent>& listener);
    template void EventManager::Unsubscribe<OnPostUpdateEvent>(const Guid& guid);
    template void EventManager::Dispatch(const OnPostUpdateEvent& event) const;
}
