#pragma once

#include "Scene.h"

class UserInterfaceScene : public Scene
{
    friend class SceneManager;

protected:
    UserInterfaceScene();
    virtual ~UserInterfaceScene();

    virtual void OnLoad();
    virtual void OnPreRender();
    virtual void OnPostRender();
};
