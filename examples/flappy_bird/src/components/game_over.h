#pragma once

#include <pluto/pluto.h>

class GameOver final : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    pluto::Vector3F currentPosition;
    pluto::Vector4F currentColor;
    pluto::SimulationManager* simulationManager;

public:
    GameOver(const pluto::Resource<pluto::GameObject>& gameObject, pluto::SimulationManager& simulationManager);

    void OnUpdate() override;
};
