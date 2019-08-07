#pragma once

#include <pluto/pluto.h>

class Rotator : public pluto::Component
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    pluto::Guid guid;
    float rotationSpeed;
    pluto::Resource<pluto::GameObject> gameObject;

public:
    Rotator(pluto::Resource<pluto::GameObject> gameObject);

    const pluto::Guid& GetId() const override;
    const std::string& GetName() const override;
    void SetName(const std::string& value) override;
    pluto::Resource<pluto::GameObject> GetGameObject() const override;

    void OnUpdate() override;
};
