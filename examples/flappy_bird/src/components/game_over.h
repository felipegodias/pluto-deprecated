#pragma once

#include <pluto/pluto.h>

class GameManager;

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
    float restartTime;
    pluto::Vector3F currentPosition;
    pluto::Vector4F currentColor;
    pluto::SimulationManager* simulationManager;
    pluto::InputManager* inputManager;
    GameManager* gameManager;

public:
    GameOver(const pluto::Resource<pluto::GameObject>& gameObject, pluto::SimulationManager& simulationManager,
             pluto::InputManager& inputManager, GameManager& gameManager);

    void OnUpdate() override;
};
