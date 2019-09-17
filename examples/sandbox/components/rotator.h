#pragma once

#include <pluto/pluto.h>

class Rotator : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    float rotationSpeed;

public:
    explicit Rotator(const pluto::Resource<pluto::GameObject>& gameObject);

    void OnUpdate() override;
};
