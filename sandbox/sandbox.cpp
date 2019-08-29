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

    cameraGo->GetTransform()->SetLocalPosition({0, 0, 10});

    Resource<Camera> camera = cameraGo->AddComponent<Camera>();

    const Resource<FontAsset> fontAsset = assetManager.Load<FontAsset>("fonts/roboto-regular.ttf");

    Resource<GameObject> logoGo = sceneManager.GetActiveScene().CreateGameObject("Logo");
    Resource<MeshRenderer> meshRenderer = logoGo->AddComponent<MeshRenderer>();
    const Resource<MeshAsset> meshAsset = assetManager.Load<MeshAsset>("meshes/quad.obj");
    meshRenderer->SetMesh(meshAsset);
    const Resource<MaterialAsset> material = assetManager.Load<MaterialAsset>("materials/pluto-logo.mat");
    meshRenderer->SetMaterial(material);
    logoGo->AddComponent<Rotator>();

    Resource<GameObject> textGo = sceneManager.GetActiveScene().CreateGameObject("TextRenderer");
    textGo->GetTransform()->SetPosition({0, 0, 2});
    Resource<TextRenderer> textRenderer = textGo->AddComponent<TextRenderer>();
    textRenderer->SetFont(fontAsset);
    textRenderer->SetText("Pluto");
    textGo->AddComponent<Rotator>();

    Resource<GameObject> logoGo2 = sceneManager.GetActiveScene().CreateGameObject("Logo");
    logoGo2->GetTransform()->SetPosition({ 0.25, 0, 3 });
    Resource<MeshRenderer> meshRenderer2 = logoGo2->AddComponent<MeshRenderer>();
    meshRenderer2->SetMesh(meshAsset);
    meshRenderer2->SetMaterial(material);
    logoGo2->AddComponent<Rotator>();
}

int main(int argc, char* argv[])
{
    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/sandbox");
    const int exitCode = pluto.Run(&OnInit);
    return exitCode;
}
