#pragma once

#include <pluto/pluto.h>

class GameManager final : public pluto::BaseService
{
public:
    class Factory final : public pluto::BaseFactory
    {
    public:
        explicit Factory(pluto::ServiceCollection& serviceCollection);

        std::unique_ptr<GameManager> Create() const;
    };

private:
    bool isGameStarted;
    bool isGameOver;

    int points;
    pluto::Guid onSceneLoadedListenerId;

    pluto::EventManager* eventManager;
    pluto::SceneManager* sceneManager;
    pluto::AssetManager* assetManager;

public:
    ~GameManager() override;

    GameManager(pluto::EventManager& eventManager, pluto::SceneManager& sceneManager,
                pluto::AssetManager& assetManager);

    void Reload();

    bool IsGameStarted() const;
    void StartGame();

    bool IsGameOver();
    void GameOver();

    int GetPoints() const;
    void IncreasePoint();

private:
    pluto::Vector3F ResolutionToScale(const pluto::Vector2F& resolution);

    void CreateBackground();

    void CreateGround();

    void CreateFlappy();

    void CratePipe(const pluto::Vector2F& position);

    void CreateFPSCounter();

    void CreateTopCollider();

    void CreatePointCounter();

    void OnSceneLoaded(const pluto::OnSceneLoadedEvent& evt);
};
