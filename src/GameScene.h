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
    void SetCoinCount(int coinCount) { this->coinCount = coinCount; }
    int GetCoinCount() { return coinCount; }

private:
    int coinCount;
};
