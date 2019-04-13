#pragma once

#include "PhysXScene.h"

class GameScene : public PhysXScene
{
    friend class SceneManager;

protected:
    GameScene();
    virtual ~GameScene();

    virtual void OnLoad();
};
