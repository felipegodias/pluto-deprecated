#include <pluto/event/event_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>

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
            std::string tag;

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
        void Subscribe(const std::string& tag, const EventListener<T>& listener)
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
            vec.push_back({tag, callback});
        }

        template <typename T, IsEvent<T>  = 0>
        void Unsubscribe(const std::string& tag)
        {
            const auto it = listeners.find(typeid(T));
            if (it == listeners.end())
            {
                return;
            }
            for (auto i = it->second.begin(); i != it->second.end(); ++i)
            {
                if (i->tag == tag)
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
    void EventManager::Subscribe(const std::string& tag, const EventListener<T>& listener)
    {
        impl->Subscribe(tag, listener);
    }

    template <typename T, IsEvent<T>>
    void EventManager::Unsubscribe(const std::string& tag)
    {
        impl->Unsubscribe<T>(tag);
    }

    template <typename T, IsEvent<T>>
    void EventManager::Dispatch(const T& event) const
    {
        impl->Dispatch(event);
    }

    template void EventManager::Subscribe(const std::string& tag, const EventListener<OnStartupEvent>& listener);
    template void EventManager::Unsubscribe<OnStartupEvent>(const std::string& tag);
    template void EventManager::Dispatch(const OnStartupEvent& event) const;

    template void EventManager::Subscribe(const std::string& tag, const EventListener<OnPreUpdateEvent>& listener);
    template void EventManager::Unsubscribe<OnPreUpdateEvent>(const std::string& tag);
    template void EventManager::Dispatch(const OnPreUpdateEvent& event) const;

    template void EventManager::Subscribe(const std::string& tag, const EventListener<OnUpdateEvent>& listener);
    template void EventManager::Unsubscribe<OnUpdateEvent>(const std::string& tag);
    template void EventManager::Dispatch(const OnUpdateEvent& event) const;

    template void EventManager::Subscribe(const std::string& tag, const EventListener<OnPostUpdateEvent>& listener);
    template void EventManager::Unsubscribe<OnPostUpdateEvent>(const std::string& tag);
    template void EventManager::Dispatch(const OnPostUpdateEvent& event) const;
}
