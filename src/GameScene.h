#pragma once

#include "Scene.h"

enum GameState
{
    GAME_STATE_INGAME_PLAYING,
    GAME_STATE_OVERLAY_TRANSITION,
    GAME_STATE_OVERLAY_PAUSE,
    GAME_STATE_CREDITS,
};

class GameScene : public Scene
{
    friend class SceneManager;

protected:
    GameScene();
    virtual ~GameScene();

    virtual void OnLoad();
    virtual void OnUpdate();

public:
    virtual void SetOption(std::string const& key, std::string const& value);

    void SetCoinCount(int coinCount);
    void SetCollectedCoinCount(int collectedCoinCount);

    int GetCoinCount() { return coinCount; }
    int GetCollectedCoinCount() { return collectedCoinCount; }

    void CompleteLevel();
    void RestartLevel();

private:
    void UpdateRemainingTime();
    void UpdateOverlayTransition();
    void EnableOverlay(bool enable);
    void UpdateDebugCommands();
    void UpdateCredits();

    GameState gameState;
    bool won;
    std::string nextLevelScene;

    int coinCount;
    int collectedCoinCount;

    float remainingTimeInSecondsOption;
    float remainingTimeInSeconds;

    float transitionTimeInSecondsOption;
    float transitionTimeInSeconds;

    bool debugContainerEnabled;
    bool debugContainerKeyPressed;
    bool toggleViewFrustumKeyPressed;

    bool autoStartNextLevel;
};
