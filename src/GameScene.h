#pragma once

#include "Scene.h"

class GameScene : public Scene
{
    friend class SceneManager;

protected:
    GameScene();
    virtual ~GameScene();

    virtual void OnLoad();

public:
    void OnGameWon();
    void OnGameLost();

    void SetCoinCount(int coinCount) { this->coinCount = coinCount; }
    void SetCollectedCoinCount(int collectedCoinCount) { this->collectedCoinCount = collectedCoinCount; }

    int GetCoinCount() { return coinCount; }
    int GetCollectedCoinCount() { return collectedCoinCount; }

private:
    int coinCount;
    int collectedCoinCount;
};
