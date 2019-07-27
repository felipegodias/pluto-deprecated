#include <pluto/window/window_manager.h>
#include <pluto/config/config_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/render/events/on_post_render_event.h>
#include <pluto/math/vector2i.h>

#include <pluto/guid.h>
#include <pluto/service/service_collection.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace pluto
{
    class WindowManager::Impl
    {
    private:
        Vector2I windowSize;

        GLFWwindow* window;

        LogManager& logManager;
        EventManager& eventManager;

        Guid onPostRenderListenerId;

    public:
        Impl(const std::string& screenTitle, Vector2I windowSize, LogManager& logManager, EventManager& eventManager) :
            windowSize(std::move(windowSize)), logManager(logManager), eventManager(eventManager)
        {
            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialize GLFW!");
            }

            window = glfwCreateWindow(this->windowSize.x, this->windowSize.y, screenTitle.c_str(), nullptr, nullptr);
            if (!window)
            {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW Window.");
            }
            glfwMakeContextCurrent(window);

            onPostRenderListenerId = eventManager.Subscribe<OnPostRenderEvent>(
                std::bind(&Impl::OnPostRender, this, std::placeholders::_1));

            logManager.LogInfo("WindowManager Initialized!");
        }

        ~Impl()
        {
            glfwTerminate();

            eventManager.Unsubscribe<OnPostRenderEvent>(onPostRenderListenerId);
            logManager.LogInfo("WindowManager Terminated!");
        }

        bool IsOpen() const
        {
            return !glfwWindowShouldClose(window);
        }

        void Close()
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        const Vector2I& GetWindowSize() const
        {
            return windowSize;
        }

        void SetWindowSize(Vector2I value)
        {
            windowSize = std::move(value);
            glfwSetWindowSize(window, windowSize.x, windowSize.y);
        }

        float GetWindowAspectRatio() const
        {
            return static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
        }

        void* GetNativeWindow() const
        {
            return window;
        }

    private:
        void OnPostRender(const OnPostRenderEvent& evt)
        {
            glfwSwapBuffers(window);
        }
    };

    WindowManager::Factory::Factory(ServiceCollection& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<WindowManager> WindowManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& configManager = serviceCollection.GetService<ConfigManager>();
        const int screenWidth = configManager.GetInt("screenWidth", 640);
        const int screenHeight = configManager.GetInt("screenHeight", 480);
        const std::string appName = configManager.GetString("appName", "Unknown");

        auto& logManager = serviceCollection.GetService<LogManager>();
        auto& eventManager = serviceCollection.GetService<EventManager>();
        return std::make_unique<WindowManager>(
            std::make_unique<Impl>(appName, Vector2I(screenWidth, screenHeight), logManager, eventManager));
    }

    WindowManager::WindowManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    WindowManager::~WindowManager() = default;

    bool WindowManager::IsOpen() const
    {
        return impl->IsOpen();
    }

    void WindowManager::Close()
    {
        impl->Close();
    }

    const Vector2I& WindowManager::GetWindowSize() const
    {
        return impl->GetWindowSize();
    }

    void WindowManager::SetWindowSize(Vector2I value)
    {
        impl->SetWindowSize(std::move(value));
    }

    float WindowManager::GetWindowAspectRatio() const
    {
        return impl->GetWindowAspectRatio();
    }

    void* WindowManager::GetNativeWindow() const
    {
        return impl->GetNativeWindow();
    }
}
