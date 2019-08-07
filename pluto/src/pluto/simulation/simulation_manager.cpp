#include <pluto/simulation/simulation_manager.h>

#include <pluto/simulation/on_pre_update_event.h>
#include <pluto/simulation/on_update_event.h>
#include <pluto/simulation/on_post_update_event.h>
#include <pluto/render/events/on_render_event.h>
#include <pluto/render/events/on_post_render_event.h>

#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/service/service_collection.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace pluto
{
    class SimulationManager::Impl
    {
        const double maxFps = 60.0;
        const double maxPeriod = 1.0 / maxFps;

        LogManager& logManager;
        EventManager& eventManager;
        double lastTime;
        double deltaTime;

        double lastSecond;
        uint16_t fps;
        uint32_t frameCount;

    public:
        Impl(LogManager& logManager, EventManager& eventManager)
            : logManager(logManager),
              eventManager(eventManager),
              lastTime(0),
              deltaTime(0),
              lastSecond(0),
              fps(0),
              frameCount(0)
        {
            logManager.LogInfo("SimulationManager initialized!");
        }

        ~Impl()
        {
            logManager.LogInfo("SimulationManager terminated!");
        }

        float GetDeltaTime() const
        {
            return deltaTime;
        }

        void Run()
        {
            const double time = glfwGetTime();
            deltaTime = time - lastTime;
            if (deltaTime >= maxPeriod)
            {
                lastTime = time;
                eventManager.Dispatch(OnPreUpdateEvent());
                eventManager.Dispatch(OnUpdateEvent());
                eventManager.Dispatch(OnPostUpdateEvent());
                eventManager.Dispatch(OnRenderEvent());
                eventManager.Dispatch(OnPostRenderEvent());
                ++fps;
                ++frameCount;
            }

            const double fpsDelta = time - lastSecond;
            if (fpsDelta > 1.0)
            {
                std::cout << fps << std::endl;
                lastSecond = time + 1.0 - fpsDelta;
                fps = 0;
            }
        }
    };

    SimulationManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<SimulationManager> SimulationManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetService<LogManager>();
        auto& eventManager = serviceCollection.GetService<EventManager>();
        return std::make_unique<SimulationManager>(std::make_unique<Impl>(logManager, eventManager));
    }

    SimulationManager::SimulationManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    SimulationManager::~SimulationManager() = default;

    float SimulationManager::GetDeltaTime() const
    {
        return impl->GetDeltaTime();
    }

    void SimulationManager::Run()
    {
        impl->Run();
    }
}
