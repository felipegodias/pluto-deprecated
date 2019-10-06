#include "game_manager.h"
#include "../components/ground.h"
#include "../components/pipe.h"
#include "../components/flappy_animation.h"
#include "../components/flappy_controller.h"
#include "../components/fps_counter.h"
#include "../components/point_counter.h"
#include "../components/intro.h"

#include "../events/on_game_over_event.h"
#include "../events/on_game_start_event.h"

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

void GameManager::Reload()
{
    sceneManager->LoadEmptyScene();
}

bool GameManager::IsGameStarted() const
{
    return isGameStarted;
}

void GameManager::StartGame()
{
    isGameStarted = true;
    eventManager->Dispatch<OnGameStartEvent>();
}

bool GameManager::IsGameOver()
{
    return isGameOver;
}

void GameManager::GameOver()
{
    isGameOver = true;
    eventManager->Dispatch<OnGameOverEvent>();
}

int GameManager::GetPoints() const
{
    return points;
}

void GameManager::IncreasePoint()
{
    ++points;
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
    groundGo->GetTransform()->SetPosition({0.083f, -0.9, 2});

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
    flappyGo->GetTransform()->SetPosition({-0.2, 0, 3});
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

void GameManager::CratePipe(const Vector2F& position)
{
    const Resource<MeshAsset> meshAsset = assetManager->Load<MeshAsset>("meshes/quad.obj");
    const Resource<MaterialAsset> material = assetManager->Load<MaterialAsset>("materials/pipe.mat");
    const Vector3F scale = ResolutionToScale({52, 320});

    Resource<GameObject> pipeContainer = sceneManager->GetActiveScene().CreateGameObject("PipeContainer");
    pipeContainer->GetTransform()->SetLocalPosition({position.x, position.y, 0,});
    pipeContainer->AddComponent<Pipe>();

    Resource<GameObject> pipeBottomGO = sceneManager->GetActiveScene().CreateGameObject(
        pipeContainer->GetTransform(), "PipeBottom");
    pipeBottomGO->GetTransform()->SetLocalPosition({0, -0.8f, 1});
    pipeBottomGO->GetTransform()->SetLocalScale(scale);

    Resource<BoxCollider2D> bottomCollider = pipeBottomGO->AddComponent<BoxCollider2D>();
    bottomCollider->SetSize({scale.x, scale.y});
    bottomCollider->SetTrigger(true);

    Resource<MeshRenderer> rendererBottom = pipeBottomGO->AddComponent<MeshRenderer>();
    rendererBottom->SetMesh(meshAsset);
    rendererBottom->SetMaterial(material);

    Resource<GameObject> pipeTopGO = sceneManager->GetActiveScene().CreateGameObject(
        pipeContainer->GetTransform(), "PipeTop");

    pipeTopGO->GetTransform()->SetLocalPosition({0, 0.8f, 1});
    pipeTopGO->GetTransform()->SetLocalScale(scale);
    pipeTopGO->GetTransform()->SetLocalRotation(Quaternion::Euler({0, 0, 180}));

    Resource<BoxCollider2D> topCollider = pipeTopGO->AddComponent<BoxCollider2D>();
    topCollider->SetSize({scale.x, scale.y});
    topCollider->SetTrigger(true);

    Resource<MeshRenderer> rendererTop = pipeTopGO->AddComponent<MeshRenderer>();
    rendererTop->SetMesh(meshAsset);
    rendererTop->SetMaterial(material);

    Resource<GameObject> pipeCheckPointGO = sceneManager->GetActiveScene().CreateGameObject(
        pipeContainer->GetTransform(), "PipeCheckPoint");

    pipeCheckPointGO->GetTransform()->SetLocalPosition(Vector3F::RIGHT * 0.085f);
    Resource<BoxCollider2D> checkPointCollider = pipeCheckPointGO->AddComponent<BoxCollider2D>();
    checkPointCollider->SetSize({0.033f, 0.45f});
    checkPointCollider->SetTrigger(true);
}

void GameManager::CreateFPSCounter()
{
    const Resource<FontAsset> fontAsset = assetManager->Load<FontAsset>("fonts/roboto-regular.ttf");

    Resource<GameObject> textGo = sceneManager->GetActiveScene().CreateGameObject("FPSCounter");
    textGo->GetTransform()->SetPosition({-0.5f, 0.91f, 5});
    textGo->GetTransform()->SetLocalScale({0.1f, 0.1f, 1});
    Resource<TextRenderer> textRenderer = textGo->AddComponent<TextRenderer>();
    textRenderer->SetFont(fontAsset);
    textRenderer->SetAnchor(TextRenderer::Anchor::UpperLeft);
    textGo->AddComponent<FPSCounter>();
}

void GameManager::CreateTopCollider()
{
    Resource<GameObject> topGo = sceneManager->GetActiveScene().CreateGameObject("Top");
    const Vector3F scale = ResolutionToScale({336, 10});
    topGo->GetTransform()->SetLocalScale(scale);
    topGo->GetTransform()->SetPosition({0, 0.95f, 0});
    Resource<BoxCollider2D> topCollider = topGo->AddComponent<BoxCollider2D>();
    topCollider->SetSize({scale.x, scale.y});
}

void GameManager::CreatePointCounter()
{
    const Resource<FontAsset> fontAsset = assetManager->Load<FontAsset>("fonts/roboto-regular.ttf");

    Resource<GameObject> containerGo = sceneManager->GetActiveScene().CreateGameObject("PointCounterContainer");
    containerGo->GetTransform()->SetPosition({0, 0.7f, 5});

    Resource<GameObject> textGo = sceneManager->GetActiveScene().CreateGameObject(
        containerGo->GetTransform(), "PointCounterText");
    textGo->GetTransform()->SetLocalScale({0.2f, 0.2f, 1});
    Resource<TextRenderer> textRenderer = textGo->AddComponent<TextRenderer>();
    textRenderer->SetFont(fontAsset);
    textRenderer->SetAnchor(TextRenderer::Anchor::MiddleCenter);

    containerGo->AddComponent<PointCounter>();
}

void GameManager::CreateIntroScreen()
{
    Resource<GameObject> introGo = sceneManager->GetActiveScene().CreateGameObject("Intro");
    const Vector3F scale = ResolutionToScale({ 184, 267});
    introGo->GetTransform()->SetLocalScale(scale);
    introGo->GetTransform()->SetLocalPosition({0, 0.25f, 4});

    Resource<MeshRenderer> groundRenderer = introGo->AddComponent<MeshRenderer>();
    const Resource<MeshAsset> meshAsset = assetManager->Load<MeshAsset>("meshes/quad.obj");
    groundRenderer->SetMesh(meshAsset);
    const Resource<MaterialAsset> material = assetManager->Load<MaterialAsset>("materials/message.mat");
    groundRenderer->SetMaterial(material);
    introGo->AddComponent<Intro>();
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
    CratePipe({0.6, 0});
    CratePipe({1.1f, 0});
    CratePipe({1.6f, 0});
    CreateFPSCounter();
    CreateTopCollider();
    CreatePointCounter();
    CreateIntroScreen();
    isGameStarted = false;
    isGameOver = false;
    points = 0;
}
