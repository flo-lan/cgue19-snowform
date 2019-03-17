#pragma once

#include "Scene.h"

class GameScene : public Scene
{
    friend class SceneManager;

protected:
    GameScene();
    virtual ~GameScene();

    virtual void OnLoad();
};
