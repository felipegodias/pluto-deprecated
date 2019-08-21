#include <pluto/pluto.h>

#include "components/rotator.h"

#include <iostream>

using namespace pluto;

void OnInit(ServiceCollection& serviceCollection)
{
    serviceCollection.AddFactory<Rotator>(std::make_unique<Rotator::Factory>(serviceCollection));

    auto& assetManager = serviceCollection.GetService<AssetManager>();

    auto& sceneManager = serviceCollection.GetService<SceneManager>();
    sceneManager.LoadEmptyScene();

    Resource<GameObject> cameraGo = sceneManager.GetActiveScene().CreateGameObject("Camera");

    cameraGo->GetTransform()->SetLocalPosition({0, 0, -1});
    Resource<Camera> camera = cameraGo->AddComponent<Camera>();

    const Resource<MeshAsset> meshAsset = assetManager.Load<MeshAsset>("meshes/quad.obj");

    const Resource<FontAsset> fontAsset = assetManager.Load<FontAsset>("fonts/roboto-regular.ttf");

    //const Resource<MaterialAsset> material = assetManager.Load<MaterialAsset>("materials/pluto-logo.mat");

    Resource<GameObject> quadGo = sceneManager.GetActiveScene().CreateGameObject("Transparent");
    quadGo->GetTransform()->SetPosition({0, 0, 0});
    Resource<MeshRenderer> meshRenderer = quadGo->AddComponent<MeshRenderer>();
    Resource<Rotator> rotator = quadGo->AddComponent<Rotator>();

    meshRenderer->SetMesh(meshAsset);
    meshRenderer->SetMaterial(fontAsset->GetMaterial());
    //meshRenderer->SetMaterial(material);
}

int main(int argc, char* argv[])
{
    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/sandbox");
    const int exitCode = pluto.Run(&OnInit);
    return exitCode;
}
