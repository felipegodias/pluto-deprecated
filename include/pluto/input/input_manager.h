#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
#include <memory>

namespace pluto
{
    enum class KeyCode;
    class Vector2;

    class PLUTO_API InputManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
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
    };
}
