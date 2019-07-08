#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
#include <memory>

namespace pluto
{
    enum class KeyCode;

    class PLUTO_API SimulationManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<SimulationManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit SimulationManager(std::unique_ptr<Impl> impl);
        ~SimulationManager();

        float GetDeltaTime() const;
        void Run();
    };
}
