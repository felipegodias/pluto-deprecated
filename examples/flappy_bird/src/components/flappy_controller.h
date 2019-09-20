#pragma once

#include <pluto/pluto.h>

class GameManager;

class FlappyController final : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);

        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    float currentAngle;
    pluto::Resource<pluto::Rigidbody2D> rigidbody;
    pluto::InputManager* inputManager;
    GameManager* gameManager;

public:
    FlappyController(const pluto::Resource<pluto::GameObject>& gameObject, pluto::InputManager& inputManager,
                     GameManager& gameManager);

    void OnUpdate() override;
};
