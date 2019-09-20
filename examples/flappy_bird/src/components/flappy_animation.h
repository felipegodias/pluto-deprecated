#pragma once

#include <pluto/pluto.h>

class FlappyAnimation final : public pluto::Behaviour
{
public:
    class Factory final : public Component::Factory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);
        std::unique_ptr<Component> Create(const pluto::Resource<pluto::GameObject>& gameObject) const override;
    };

private:
    float currentAnimationTime;
    float animationThreshold;
    int currentTexture;

    pluto::Resource<pluto::MaterialAsset> material;
    std::vector<pluto::Resource<pluto::TextureAsset>> textures;
    pluto::SimulationManager* simulationManager;

public:
    FlappyAnimation(const pluto::Resource<pluto::GameObject>& gameObject, pluto::SimulationManager& simulationManager,
                    pluto::AssetManager& assetManager);

    void OnUpdate() override;
};
