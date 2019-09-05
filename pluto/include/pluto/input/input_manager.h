#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Vector2F;
    enum class KeyCode;

    class PLUTO_API InputManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<InputManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit InputManager(std::unique_ptr<Impl> impl);
        ~InputManager();

        bool AnyKey() const;
        bool AnyKeyDown() const;
        bool GetKey(KeyCode keyCode) const;
        bool GetKeyDown(KeyCode keyCode) const;
        bool GetKeyUp(KeyCode keyCode) const;

        Vector2F GetMousePosition() const;
        Vector2F GetMouseScrollDelta() const;

        void MainLoop();
    };
}
