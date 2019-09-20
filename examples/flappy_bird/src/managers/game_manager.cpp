#include "game_manager.h"
#include "../components/ground.h"
#include "../components/flappy_animation.h"
#include "../components/flappy_controller.h"

using namespace pluto;

GameManager::Factory::Factory(ServiceCollection& serviceCollection)
    : BaseFactory(serviceCollection)
{
}

std::unique_ptr<GameManager> GameManager::Factory::Create() const
{
    auto& eventManager = GetServiceCollection().GetService<EventManager>();
    auto& sceneManager = GetServiceCollection().GetService<SceneManager>();
    auto& assetManager = GetServiceCollection().GetService<AssetManager>();
    return std::make_unique<GameManager>(eventManager, sceneManager, assetManager);
}

GameManager::~GameManager()
{
    eventManager->Unsubscribe<OnSceneLoadedEvent>(onSceneLoadedListenerId);
}

GameManager::GameManager(EventManager& eventManager, SceneManager& sceneManager, AssetManager& assetManager)
    : eventManager(&eventManager),
      sceneManager(&sceneManager),
      assetManager(&assetManager)
{
    onSceneLoadedListenerId = eventManager.Subscribe(*this, &GameManager::OnSceneLoaded);
}

bool GameManager::IsPlaying() const
{
    return isPlaying;
}

void GameManager::Reload()
{
    sceneManager->LoadEmptyScene();
}

void GameManager::Pause()
{
    isPlaying = false;
}

void GameManager::UnPause()
{
    isPlaying = true;
}

Vector3F GameManager::ResolutionToScale(const Vector2F& resolution)
{
    return {resolution.x / 288, resolution.y / 512 * 1.7777f, 1};
}

void GameManager::CreateBackground()
{
    Resource<GameObject> backgroundGo = sceneManager->GetActiveScene().CreateGameObject("Background");
    backgroundGo->GetTransform()->SetLocalScale(ResolutionToScale({288, 512}));

    Resource<MeshRenderer> backgroundRenderer = backgroundGo->AddComponent<MeshRenderer>();
    const Resource<MeshAsset> meshAsset = assetManager->Load<MeshAsset>("meshes/quad.obj");
    backgroundRenderer->SetMesh(meshAsset);
    const Resource<MaterialAsset> material = assetManager->Load<MaterialAsset>("materials/background-day.mat");
    backgroundRenderer->SetMaterial(material);
}

void GameManager::CreateGround()
{
    Resource<GameObject> groundGo = sceneManager->GetActiveScene().CreateGameObject("Ground");
    const Vector3F scale = ResolutionToScale({336, 112});
    groundGo->GetTransform()->SetLocalScale(scale);
    groundGo->GetTransform()->SetPosition({0.083f, -0.7, 1});

    Resource<MeshRenderer> groundRenderer = groundGo->AddComponent<MeshRenderer>();
    const Resource<MeshAsset> meshAsset = assetManager->Load<MeshAsset>("meshes/quad.obj");
    groundRenderer->SetMesh(meshAsset);
    const Resource<MaterialAsset> material = assetManager->Load<MaterialAsset>("materials/ground.mat");
    groundRenderer->SetMaterial(material);
    groundGo->AddComponent<Ground>();
    Resource<BoxCollider2D> boxCollider2D = groundGo->AddComponent<BoxCollider2D>();
    boxCollider2D->SetSize({scale.x, scale.y});
}

void GameManager::CreateFlappy()
{
    Resource<GameObject> flappyGo = sceneManager->GetActiveScene().CreateGameObject("Flappy");
    flappyGo->GetTransform()->SetPosition({0, 0, 2});
    Resource<MeshRenderer> renderer = flappyGo->AddComponent<MeshRenderer>();
    const Resource<MeshAsset> meshAsset = assetManager->Load<MeshAsset>("meshes/quad.obj");
    renderer->SetMesh(meshAsset);
    const Resource<MaterialAsset> material = assetManager->Load<MaterialAsset>("materials/flappy.mat");
    renderer->SetMaterial(material);
    flappyGo->AddComponent<FlappyAnimation>();
    flappyGo->AddComponent<Rigidbody2D>();
    flappyGo->AddComponent<FlappyController>();
    Resource<CircleCollider2D> collider = flappyGo->AddComponent<CircleCollider2D>();
    collider->SetRadius(0.04f);
}

void GameManager::OnSceneLoaded(const OnSceneLoadedEvent& evt)
{
    Resource<GameObject> cameraGo = sceneManager->GetActiveScene().CreateGameObject("Camera");

    cameraGo->GetTransform()->SetLocalPosition({0, 0, 10});

    Resource<Camera> camera = cameraGo->AddComponent<Camera>();
    camera->SetOrthographicSize(0.8888f);

    CreateBackground();
    CreateGround();
    CreateFlappy();
    isPlaying = true;
}
