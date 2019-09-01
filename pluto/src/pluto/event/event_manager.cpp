#include <pluto/event/event_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/service/service_collection.h>

#include <pluto/guid.h>

#include <pluto/event/on_startup_event.h>
#include <pluto/simulation/on_pre_update_event.h>
#include <pluto/simulation/on_update_event.h>
#include <pluto/simulation/on_post_update_event.h>
#include <pluto/asset/events/on_asset_unload_event.h>

#include <pluto/render/events/on_render_event.h>
#include <pluto/render/events/on_post_render_event.h>

#include <memory>
#include <typeindex>
#include <vector>
#include <unordered_map>

namespace pluto
{
    class EventManager::Impl
    {
        struct EventListenerEntry final
        {
            Guid guid;
            EventListener<BaseEvent> callback;
        };

        LogManager* logManager;
        std::unordered_map<std::type_index, std::vector<EventListenerEntry>> listeners;

    public:
        explicit Impl(LogManager& logManager)
            : logManager(&logManager)
        {
            logManager.LogInfo("EventManager Initialized!");
        }

        ~Impl()
        {
            logManager->LogInfo("EventManager Terminated!");
        }

        Guid Subscribe(const std::type_info& eventType, const EventListener<BaseEvent>& listener)
        {
            const auto it = listeners.find(eventType);
            if (it == listeners.end())
            {
                listeners[eventType] = std::vector<EventListenerEntry>();
            }

            auto& vec = listeners[eventType];
            const Guid guid = Guid::New();
            vec.push_back({guid, listener});
            return guid;
        }

        void Unsubscribe(const std::type_info& eventType, const Guid& guid)
        {
            const auto it = listeners.find(eventType);
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

        void Dispatch(const BaseEvent& event) const
        {
            const auto it = listeners.find(typeid(event));
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

    EventManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<EventManager> EventManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetService<LogManager>();
        return std::make_unique<EventManager>(std::make_unique<Impl>(logManager));
    }

    EventManager::~EventManager() = default;

    EventManager::EventManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    EventManager::EventManager(EventManager&& other) noexcept = default;

    EventManager& EventManager::operator=(EventManager&& other) noexcept = default;

    Guid EventManager::Subscribe(const std::type_info& eventType, const EventListener<BaseEvent>& listener)
    {
        return impl->Subscribe(eventType, listener);
    }

    void EventManager::Unsubscribe(const std::type_info& eventType, const Guid& guid)
    {
        impl->Unsubscribe(eventType, guid);
    }

    void EventManager::Dispatch(const BaseEvent& event) const
    {
        impl->Dispatch(event);
    }
}
