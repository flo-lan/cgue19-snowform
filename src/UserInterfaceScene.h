#pragma once

#include "Scene.h"

class UserInterfaceScene : public Scene
{
    friend class SceneManager;

protected:
    UserInterfaceScene();
    virtual ~UserInterfaceScene();

    virtual void OnLoad();
    virtual void OnUpdate();
    virtual void OnPreRender();
    virtual void OnPostRender();

public:
    void SetCoinCountText(std::string const& text);
    void SetRemainingTimeText(std::string const& text);
    void EnableLevelFinishedText(bool enable);
    void EnableCoinsLeftText(bool enable);
    void EnableFallenToDeathText(bool enable);
    void EnableTimeIsUpText(bool enable);
    void EnableTransitionOverlay(bool enable);
    void SetTransitionOverlayCutOff(float cutOff);
    void DisableOverlayTexts();
    void ScrollCredits(float yScroll);
    void EnableDebugContainer(bool enable);
    void SetObjectsRenderedText(std::string const& text);
    void SetObjectsCulledText(std::string const& text);
    void SetFPSText(std::string const& text);
};
