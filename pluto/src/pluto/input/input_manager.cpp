#include <pluto/input/input_manager.h>
#include <pluto/input/key_code.h>
#include <pluto/log/log_manager.h>
#include <pluto/window/window_manager.h>
#include <pluto/service/service_collection.h>

#include <pluto/event/event_manager.h>
#include <pluto/simulation/on_pre_update_event.h>

#include <pluto/guid.h>
#include <pluto/math/vector2f.h>

#include <unordered_set>
#include <GLFW/glfw3.h>

namespace pluto
{
    class InputManager::Impl
    {
        LogManager& logManager;
        EventManager& eventManager;

        std::unordered_set<KeyCode> keys;
        std::unordered_set<KeyCode> keysDown;
        std::unordered_set<KeyCode> keysUp;

        double mousePositionX;
        double mousePositionY;
        double mouseScrollDeltaX;
        double mouseScrollDeltaY;

        Guid onPreUpdateEventGuid;

    public:
        Impl(LogManager& logManager, EventManager& eventManager, GLFWwindow* window) : logManager(logManager),
                                                                                       eventManager(eventManager),
                                                                                       mousePositionX(0),
                                                                                       mousePositionY(0),
                                                                                       mouseScrollDeltaX(0),
                                                                                       mouseScrollDeltaY(0)
        {
            static Impl* instance = this;

            onPreUpdateEventGuid = eventManager.Subscribe<OnPreUpdateEvent>([&](const OnPreUpdateEvent& event)
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

            glfwSetCursorPosCallback(window, [](GLFWwindow* window, const double posX, const double posY)
            {
                instance->mousePositionX = posX;
                instance->mousePositionY = posY;
            });

            glfwSetScrollCallback(window, [](GLFWwindow* window, const double xOffset, const double yOffset)
            {
                instance->mouseScrollDeltaX = xOffset;
                instance->mouseScrollDeltaY = yOffset;
            });

            logManager.LogInfo("InputManager initialized!");
        }

        ~Impl()
        {
            eventManager.Unsubscribe<OnPreUpdateEvent>(onPreUpdateEventGuid);
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

        Vector2F GetMousePosition() const
        {
            return Vector2F(static_cast<float>(mousePositionX), static_cast<float>(mousePositionY));
        }

        Vector2F GetMouseScrollDelta() const
        {
            return Vector2F(static_cast<float>(mouseScrollDeltaX), static_cast<float>(mouseScrollDeltaY));
        }

    private:
        void OnPreUpdate(const OnPreUpdateEvent& event)
        {
            mouseScrollDeltaX = 0;
            mouseScrollDeltaY = 0;
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

    InputManager::Factory::Factory(ServiceCollection& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<InputManager> InputManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetSingleton<LogManager>();
        auto& eventManager = serviceCollection.GetSingleton<EventManager>();
        auto& windowManager = serviceCollection.GetSingleton<WindowManager>();
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

    Vector2F InputManager::GetMousePosition() const
    {
        return impl->GetMousePosition();
    }

    Vector2F InputManager::GetMouseScrollDelta() const
    {
        return impl->GetMouseScrollDelta();
    }
}
