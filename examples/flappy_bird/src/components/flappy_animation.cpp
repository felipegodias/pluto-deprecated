#include "flappy_animation.h"

using namespace pluto;

Vector3F GetTextureScale(Resource<TextureAsset> texture)
{
    return {texture->GetWidth() / 288.0f, texture->GetHeight() / 512.0f * 1.7777f, 1};
}

FlappyAnimation::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> FlappyAnimation::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& simulationManager = GetServiceCollection().GetService<SimulationManager>();
    auto& assetManager = GetServiceCollection().GetService<AssetManager>();
    return std::make_unique<FlappyAnimation>(gameObject, simulationManager, assetManager);
}

FlappyAnimation::FlappyAnimation(const Resource<GameObject>& gameObject,
                                 SimulationManager& simulationManager, AssetManager& assetManager)
    : Behaviour(gameObject),
      currentAnimationTime(0),
      animationThreshold(0.1f),
      currentTexture(0),
      simulationManager(&simulationManager)
{
    material = gameObject->GetComponent<Renderer>()->GetMaterial();

    textures.push_back(assetManager.Load<TextureAsset>("textures/yellowbird-upflap.png"));
    textures.push_back(assetManager.Load<TextureAsset>("textures/yellowbird-midflap.png"));
    textures.push_back(assetManager.Load<TextureAsset>("textures/yellowbird-downflap.png"));
    material->SetTexture("u_mat.mainTex", textures[currentTexture]);
    GetGameObject()->GetTransform()->SetLocalScale(GetTextureScale(textures[currentTexture]));
}

void FlappyAnimation::OnUpdate()
{
    currentAnimationTime += simulationManager->GetDeltaTime();
    if (currentAnimationTime > animationThreshold)
    {
        currentAnimationTime = 0;
        int index = currentTexture++;
        material->SetTexture("u_mat.mainTex", textures[index]);
        GetGameObject()->GetTransform()->SetLocalScale(GetTextureScale(textures[index]));
        currentTexture = currentTexture >= textures.size() ? 0 : currentTexture;
    }
}
