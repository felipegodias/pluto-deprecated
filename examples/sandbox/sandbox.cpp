#include <pluto/pluto.h>

#include "components/rotator.h"

#include <iostream>

using namespace pluto;

void OnInit(ServiceCollection& serviceCollection)
{
    serviceCollection.EmplaceFactory<Rotator>();

    auto& assetManager = serviceCollection.GetService<AssetManager>();

    auto& sceneManager = serviceCollection.GetService<SceneManager>();
    sceneManager.LoadEmptyScene();

    Resource<GameObject> cameraGo = sceneManager.GetActiveScene().CreateGameObject("Camera");

    cameraGo->GetTransform()->SetLocalPosition({0, 0, 10});

    Resource<Camera> camera = cameraGo->AddComponent<Camera>();

    const Resource<FontAsset> fontAsset = assetManager.Load<FontAsset>("fonts/roboto-regular.ttf");

    Resource<GameObject> logoGo = sceneManager.GetActiveScene().CreateGameObject("Logo");
    logoGo->GetTransform()->SetLocalPosition({ 0, 5, 0 });

    Resource<MeshRenderer> meshRenderer = logoGo->AddComponent<MeshRenderer>();
    const Resource<MeshAsset> meshAsset = assetManager.Load<MeshAsset>("meshes/quad.obj");
    meshRenderer->SetMesh(meshAsset);
    const Resource<MaterialAsset> material = assetManager.Load<MaterialAsset>("materials/pluto-logo.mat");
    meshRenderer->SetMaterial(material);
    logoGo->AddComponent<Rigidbody2D>();
    Resource<CircleCollider2D> circleCollider = logoGo->AddComponent<CircleCollider2D>();
    circleCollider->SetRadius(0.5f);

    Resource<GameObject> textGo = sceneManager.GetActiveScene().CreateGameObject("TextRenderer");
    textGo->GetTransform()->SetPosition({0, 0, 0});
    Resource<TextRenderer> textRenderer = textGo->AddComponent<TextRenderer>();
    textRenderer->SetFont(fontAsset);
    textRenderer->SetText("Pluto");
    textRenderer->SetAnchor(TextRenderer::Anchor::MiddleCenter);
    Resource<BoxCollider2D> boxCollider = textGo->AddComponent<BoxCollider2D>();
    boxCollider->SetSize({ 2, 0.5f });
    //textGo->AddComponent<Rotator>();
}

int main(int argc, char* argv[])
{
    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/sandbox");
    const int exitCode = pluto.Run(&OnInit);
    return exitCode;
}
