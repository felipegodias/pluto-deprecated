#pragma once

#include <pluto/pluto.h>

class GameManager;

class Pipe final : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    float moveSpeed = 0.25f;
    pluto::SimulationManager* simulationManager;
    GameManager* gameManager;

public:
    Pipe(const pluto::Resource<pluto::GameObject>& gameObject, pluto::SimulationManager& simulationManager,
         GameManager& gameManager);

    void OnUpdate() override;
};
