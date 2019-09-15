#include <pluto/simulation/simulation_manager.h>
#include <pluto/simulation/events/on_main_loop_begin.h>
#include <pluto/simulation/events/on_main_loop_end.h>

#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>

#include "pluto/physics_2d/events/on_early_fixed_update_event.h"
#include "pluto/physics_2d/events/on_late_fixed_update_event.h"
#include "pluto/physics_2d/events/on_fixed_update_event.h"

#include "pluto/render/events/on_post_render_event.h"
#include "pluto/render/events/on_pre_render_event.h"
#include "pluto/render/events/on_render_event.h"

#include <pluto/input/input_manager.h>
#include <pluto/scene/scene_manager.h>
#include <pluto/service/service_collection.h>
#include <GLFW/glfw3.h>

namespace pluto
{
    class SimulationManager::Impl
    {
        const float maxFps = 60.0;
        const float maxPeriod = 1.0 / maxFps;

        const float maxFixedPeriod = 0.02f;

        LogManager* logManager;
        EventManager* eventManager;

        InputManager* inputManager;
        SceneManager* sceneManager;

        float lastTime;
        float deltaTime;

        float lastFixedTime;

    public:
        ~Impl()
        {
            logManager->LogInfo("SimulationManager terminated!");
        }

        Impl(LogManager& logManager, EventManager& eventManager, InputManager& inputManager, SceneManager& sceneManager)
            : logManager(&logManager),
              eventManager(&eventManager),
              inputManager(&inputManager),
              sceneManager(&sceneManager),
              lastTime(0),
              deltaTime(0),
              lastFixedTime(0)
        {
            logManager.LogInfo("SimulationManager initialized!");
        }

        float GetDeltaTime() const
        {
            return deltaTime;
        }

        void MainLoop()
        {
            const auto time = static_cast<float>(glfwGetTime());
            if (lastTime == 0)
            {
                lastTime = time;
            }
            if (lastFixedTime == 0)
            {
                lastFixedTime = time;
            }

            deltaTime = time - lastTime;
            const bool shouldCallUpdate = deltaTime >= maxPeriod;
            const float fixedDeltaTime = time - lastFixedTime;
            const bool shouldCallFixedUpdate = fixedDeltaTime >= maxFixedPeriod;

            if (shouldCallUpdate || shouldCallFixedUpdate)
            {
                eventManager->Dispatch<OnMainLoopBeginEvent>();
            }

            if (shouldCallUpdate)
            {
                lastTime = time - (deltaTime - maxPeriod);

                inputManager->MainLoop();
                sceneManager->MainLoop();
            }

            if (shouldCallFixedUpdate)
            {
                const int stepCount = floor(fixedDeltaTime / maxFixedPeriod);
                lastFixedTime = time - (fixedDeltaTime - static_cast<float>(stepCount) * maxFixedPeriod);
                for (int i = 0; i < stepCount; ++i)
                {
                    eventManager->Dispatch<OnEarlyFixedUpdateEvent>();
                    eventManager->Dispatch<OnFixedUpdateEvent>();
                    eventManager->Dispatch<OnLateFixedUpdateEvent>();
                }
            }

            if (shouldCallUpdate)
            {
                eventManager->Dispatch<OnPreRenderEvent>();
                eventManager->Dispatch<OnRenderEvent>();
                eventManager->Dispatch<OnPostRenderEvent>();
            }

            if (shouldCallUpdate || shouldCallFixedUpdate)
            {
                eventManager->Dispatch<OnMainLoopEndEvent>();
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

        auto& inputManager = serviceCollection.GetService<InputManager>();
        auto& sceneManager = serviceCollection.GetService<SceneManager>();

        return std::make_unique<SimulationManager>(
            std::make_unique<Impl>(logManager, eventManager, inputManager, sceneManager));
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

    void SimulationManager::MainLoop()
    {
        impl->MainLoop();
    }
}
