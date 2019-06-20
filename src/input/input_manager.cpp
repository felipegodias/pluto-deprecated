#include <pluto/input/input_manager.h>
#include <pluto/input/key_code.h>
#include <pluto/log/log_manager.h>
#include <pluto/window/window_manager.h>
#include <pluto/di/di_container.h>

#include <pluto/event/event_manager.h>
#include <pluto/simulation/on_pre_update_event.h>

#include <unordered_set>
#include <GLFW/glfw3.h>

namespace pluto
{
    class InputManager::Impl
    {
    private:
        static inline const std::string ON_PRE_UPDATE_EVENT_TAG = "InputManager::OnPreUpdateEvent::OnPreUpdate";

        LogManager& logManager;
        EventManager& eventManager;

        std::unordered_set<KeyCode> keys;
        std::unordered_set<KeyCode> keysDown;
        std::unordered_set<KeyCode> keysUp;

    public:
        Impl(LogManager& logManager, EventManager& eventManager, GLFWwindow* window) : logManager(logManager),
                                                                                       eventManager(eventManager)
        {
            static Impl* instance = this;

            eventManager.Subscribe<OnPreUpdateEvent>(ON_PRE_UPDATE_EVENT_TAG, [&](const OnPreUpdateEvent& event)
            {
                OnPreUpdate(event);
            });

            glfwSetKeyCallback(window, [](GLFWwindow* window, const int key, const int scanCode, const int action,
                                          const int mods)
            {
                const auto keyCode = static_cast<KeyCode>(key);
                instance->HandleKeyCode(keyCode, action);
            });

            glfwSetMouseButtonCallback(
                window, [](GLFWwindow* window, const int button, const int action, const int mods)
                {
                    instance->HandleMouseButton(button, action);
                });

            logManager.LogInfo("InputManager initialized!");
        }

        ~Impl()
        {
            eventManager.Unsubscribe<OnPreUpdateEvent>(ON_PRE_UPDATE_EVENT_TAG);
            logManager.LogInfo("InputManager terminated!");
        }

        bool AnyKey() const
        {
            return !keys.empty();
        }

        bool AnyKeyDown() const
        {
            return !keysDown.empty();
        }

        bool GetKey(const KeyCode keyCode) const
        {
            return keys.find(keyCode) != keys.end();
        }

        bool GetKeyDown(const KeyCode keyCode) const
        {
            return keysDown.find(keyCode) != keysDown.end();
        }

        bool GetKeyUp(const KeyCode keyCode) const
        {
            return keysUp.find(keyCode) != keysUp.end();
        }

    private:
        void OnPreUpdate(const OnPreUpdateEvent& event)
        {
            keysDown.clear();
            keysUp.clear();
            glfwPollEvents();
        }

        void HandleKeyCode(const KeyCode keyCode, const int action)
        {
            if (action == GLFW_PRESS)
            {
                keys.emplace(keyCode);
                keysDown.emplace(keyCode);
            }
            else if (action == GLFW_RELEASE)
            {
                keys.erase(keyCode);
                keysUp.emplace(keyCode);
            }
        }

        void HandleMouseButton(const int mouseButton, const int action)
        {
            if (mouseButton == GLFW_MOUSE_BUTTON_1)
            {
                HandleKeyCode(KeyCode::MouseButton0, action);
            }
            else if (mouseButton == GLFW_MOUSE_BUTTON_2)
            {
                HandleKeyCode(KeyCode::MouseButton1, action);
            }
            else if (mouseButton == GLFW_MOUSE_BUTTON_3)
            {
                HandleKeyCode(KeyCode::MouseButton2, action);
            }
        }
    };

    InputManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<InputManager> InputManager::Factory::Create() const
    {
        auto& logManager = diContainer.GetSingleton<LogManager>();
        auto& eventManager = diContainer.GetSingleton<EventManager>();
        auto& windowManager = diContainer.GetSingleton<WindowManager>();
        auto window = static_cast<GLFWwindow*>(windowManager.GetNativeWindow());
        return std::make_unique<InputManager>(std::make_unique<Impl>(logManager, eventManager, window));
    }

    InputManager::InputManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    InputManager::~InputManager() = default;

    bool InputManager::AnyKey() const
    {
        return impl->AnyKey();
    }

    bool InputManager::AnyKeyDown() const
    {
        return impl->AnyKeyDown();
    }

    bool InputManager::GetKey(const KeyCode keyCode) const
    {
        return impl->GetKey(keyCode);
    }

    bool InputManager::GetKeyDown(const KeyCode keyCode) const
    {
        return impl->GetKeyDown(keyCode);
    }

    bool InputManager::GetKeyUp(const KeyCode keyCode) const
    {
        return impl->GetKeyUp(keyCode);
    }
}
