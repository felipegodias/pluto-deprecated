#pragma once

#include <pluto/pluto.h>

class GameManager;

class Intro final : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    GameManager* gameManager;

public:
    Intro(const pluto::Resource<pluto::GameObject>& gameObject, GameManager& gameManager);

    void OnUpdate() override;
};
