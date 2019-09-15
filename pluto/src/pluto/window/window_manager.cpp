#include <pluto/window/window_manager.h>
#include <pluto/config/config_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/math/vector2i.h>

#include <pluto/service/service_collection.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace pluto
{
    class WindowManager::Impl
    {
        Vector2I windowSize;

        GLFWwindow* window;

        LogManager& logManager;

    public:
        ~Impl()
        {
            glfwTerminate();
            logManager.LogInfo("WindowManager Terminated!");
        }

        Impl(const std::string& screenTitle, const Vector2I& windowSize, LogManager& logManager)
            : windowSize(windowSize),
              logManager(logManager)
        {
            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialize GLFW!");
            }

            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
            window = glfwCreateWindow(this->windowSize.x, this->windowSize.y, screenTitle.c_str(), nullptr, nullptr);
            if (!window)
            {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW Window.");
            }
            glfwMakeContextCurrent(window);
            glfwSwapInterval(0);

            logManager.LogInfo("WindowManager Initialized!");
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

        void SetWindowSize(const Vector2I& value)
        {
            windowSize = value;
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

        void SwapBuffers()
        {
            glfwSwapBuffers(window);
        }
    };

    WindowManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
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
        return std::make_unique<WindowManager>(
            std::make_unique<Impl>(appName, Vector2I(screenWidth, screenHeight), logManager));
    }

    WindowManager::~WindowManager() = default;

    WindowManager::WindowManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    WindowManager::WindowManager(WindowManager&& other) noexcept = default;

    WindowManager& WindowManager::operator=(WindowManager&& rhs) noexcept = default;

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

    void WindowManager::SetWindowSize(const Vector2I& value)
    {
        impl->SetWindowSize(value);
    }

    float WindowManager::GetWindowAspectRatio() const
    {
        return impl->GetWindowAspectRatio();
    }

    void* WindowManager::GetNativeWindow() const
    {
        return impl->GetNativeWindow();
    }

    void WindowManager::SwapBuffers()
    {
        impl->SwapBuffers();
    }
}
