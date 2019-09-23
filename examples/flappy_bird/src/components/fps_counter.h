#pragma once

#include <pluto/pluto.h>

class FPSCounter final : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    int fps;
    float time;
    pluto::Resource<pluto::TextRenderer> textRenderer;
    pluto::SimulationManager* simulationManager;

public:
    FPSCounter(const pluto::Resource<pluto::GameObject>& gameObject, pluto::SimulationManager& simulationManager);

    void OnUpdate() override;
};
