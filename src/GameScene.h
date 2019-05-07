#pragma once

#include "Scene.h"

class GameScene : public Scene
{
    friend class SceneManager;

protected:
    GameScene();
    virtual ~GameScene();

    virtual void OnLoad();
    virtual void OnUpdate();

public:
    void OnGameWon();
    void OnGameLost();

    virtual void SetOption(std::string const& key, std::string const& value);

    void SetCoinCount(int coinCount) { this->coinCount = coinCount; }
    void SetCollectedCoinCount(int collectedCoinCount) { this->collectedCoinCount = collectedCoinCount; }

    int GetCoinCount() { return coinCount; }
    int GetCollectedCoinCount() { return collectedCoinCount; }

private:
    int coinCount;
    int collectedCoinCount;

    float remainingTimeInSecondsOption;
    float remainingTimeInSeconds;
};
