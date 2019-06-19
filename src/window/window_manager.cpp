#include <pluto/window/window_manager.h>
#include <pluto/config/config_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace pluto
{
    class WindowManager::Impl
    {
    private:
        LogManager& logManager;
        GLFWwindow* window;

    public:
        explicit Impl(const ConfigManager& configManager, LogManager& logManager) : logManager(logManager)
        {
            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialize GLFW!");
            }

            const int screenWidth = configManager.GetInt("screenWidth", 640);
            const int screenHeight = configManager.GetInt("screenHeight", 480);
            const std::string appName = configManager.GetString("appName", "Unknown");

            window = glfwCreateWindow(screenWidth, screenHeight, appName.c_str(), nullptr, nullptr);
            if (!window)
            {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW Window.");
            }
            glfwMakeContextCurrent(window);
            logManager.LogInfo("WindowManager Initialized!");
        }

        ~Impl()
        {
            glfwTerminate();
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

        size_t GetWidth() const
        {
            int width;
            glfwGetWindowSize(window, &width, nullptr);
            return width;
        }

        size_t GetHeight() const
        {
            int height;
            glfwGetWindowSize(window, nullptr, &height);
            return height;
        }

        void SetSize(const size_t width, const size_t height)
        {
            glfwSetWindowSize(window, width, height);
        }
    };

    WindowManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<WindowManager> WindowManager::Factory::Create() const
    {
        auto& configManager = diContainer.GetSingleton<ConfigManager>();
        auto& logManager = diContainer.GetSingleton<LogManager>();
        return std::make_unique<WindowManager>(std::make_unique<Impl>(configManager, logManager));
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

    size_t WindowManager::GetWidth() const
    {
        return impl->GetWidth();
    }

    size_t WindowManager::GetHeight() const
    {
        return impl->GetHeight();
    }

    void WindowManager::SetSize(const size_t width, const size_t height)
    {
        impl->SetSize(width, height);
    }
}
