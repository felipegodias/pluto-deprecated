#include <pluto/simulation/simulation_manager.h>

#include <pluto/log/log_manager.h>
#include <pluto/input/input_manager.h>
#include <pluto/scene/scene_manager.h>
#include <pluto/physics_2d/physics_2d_manager.h>
#include <pluto/render/render_manager.h>
#include <pluto/service/service_collection.h>
#include <GLFW/glfw3.h>

namespace pluto
{
    class SimulationManager::Impl
    {
        const double maxFps = 60.0;
        const double maxPeriod = 1.0 / maxFps;

        LogManager* logManager;
        InputManager* inputManager;
        SceneManager* sceneManager;
        Physics2DManager* physics2DManager;
        RenderManager* renderManager;

        double lastTime;
        double deltaTime;

        double lastSecond;
        uint16_t fps;
        uint32_t frameCount;

    public:
        Impl(LogManager& logManager, InputManager& inputManager, SceneManager& sceneManager,
             Physics2DManager& physics2DManager,
             RenderManager& renderManager)
            : logManager(&logManager),
              inputManager(&inputManager),
              sceneManager(&sceneManager),
              physics2DManager(&physics2DManager),
              renderManager(&renderManager),
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
            logManager->LogInfo("SimulationManager terminated!");
        }

        float GetDeltaTime() const
        {
            return static_cast<float>(deltaTime);
        }

        void Run()
        {
            const double time = glfwGetTime();
            deltaTime = time - lastTime;
            if (deltaTime >= maxPeriod)
            {
                lastTime = time;

                inputManager->MainLoop();
                sceneManager->MainLoop();
                physics2DManager->MainLoop(static_cast<float>(deltaTime));
                renderManager->MainLoop();

                ++fps;
                ++frameCount;
            }

            const double fpsDelta = time - lastSecond;
            if (fpsDelta > 1.0)
            {
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
        auto& inputManager = serviceCollection.GetService<InputManager>();
        auto& sceneManager = serviceCollection.GetService<SceneManager>();
        auto& physics2DManager = serviceCollection.GetService<Physics2DManager>();
        auto& renderManager = serviceCollection.GetService<RenderManager>();

        return std::make_unique<SimulationManager>(
            std::make_unique<Impl>(logManager, inputManager, sceneManager, physics2DManager, renderManager));
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
