#include "UserInterfaceScene.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ImageCutOffMaterial.h"
#include "TextComponent.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "Time.h"
#include <GL\glew.h>

UserInterfaceScene::UserInterfaceScene() :
    Scene::Scene("User Interface Scene")
{
}

UserInterfaceScene::~UserInterfaceScene()
{
}

void UserInterfaceScene::OnLoad()
{
}

void UserInterfaceScene::OnUpdate()
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("FPS"))
    {
        std::string fps = std::to_string((int)round(sTime.GetFPS()));
        textComponent->SetText(fps);
    }
}

void UserInterfaceScene::OnPreRender()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UserInterfaceScene::OnPostRender()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void UserInterfaceScene::SetCoinCountText(std::string const& text)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("CoinCount"))
    {
        textComponent->SetText(text);
    }
}

void UserInterfaceScene::SetRemainingTimeText(std::string const& text)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("RemainingTime"))
    {
        textComponent->SetText(text);
    }
}

void UserInterfaceScene::EnableLevelFinishedText(bool enable)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("LevelFinished"))
    {
        textComponent->SetEnabled(enable);
    }
}

void UserInterfaceScene::EnableCoinsLeftText(bool enable)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("CoinsLeft"))
    {
        textComponent->SetEnabled(enable);
    }
}

void UserInterfaceScene::EnableFallenToDeathText(bool enable)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("FallenToDeath"))
    {
        textComponent->SetEnabled(enable);
    }
}

void UserInterfaceScene::EnableTimeIsUpText(bool enable)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("TimeIsUp"))
    {
        textComponent->SetEnabled(enable);
    }
}

void UserInterfaceScene::EnableTransitionOverlay(bool enable)
{
    if (MeshRendererComponent* meshRenderer = GetComponentByGameObjectId<MeshRendererComponent>("TransitionOverlay"))
    {
        meshRenderer->SetEnabled(enable);
    }
}

void UserInterfaceScene::SetTransitionOverlayCutOff(float cutOff)
{
    if (MeshRendererComponent* meshRenderer = GetComponentByGameObjectId<MeshRendererComponent>("TransitionOverlay"))
    {
        if (ImageCutOffMaterial* im = static_cast<ImageCutOffMaterial*>(meshRenderer->GetMaterial()))
        {
            im->SetCutOff(cutOff);
        }
    }
}

void UserInterfaceScene::DisableOverlayTexts()
{
    EnableLevelFinishedText(false);
    EnableCoinsLeftText(false);
    EnableFallenToDeathText(false);
    EnableTimeIsUpText(false);
}

void UserInterfaceScene::ScrollCredits(float yScroll)
{
    if (TransformComponent* transform = GetComponentByGameObjectId<TransformComponent>("CreditsContainer"))
    {
        transform->SetLocalPositionY(transform->GetLocalPositionY() + yScroll);
    }
}

void UserInterfaceScene::EnableDebugContainer(bool enable)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("ObjectsRenderedLabel"))
    {
        textComponent->SetEnabled(enable);
    }

    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("ObjectsCulledLabel"))
    {
        textComponent->SetEnabled(enable);
    }

    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("ObjectsRendered"))
    {
        textComponent->SetEnabled(enable);
    }

    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("ObjectsCulled"))
    {
        textComponent->SetEnabled(enable);
    }
}

void UserInterfaceScene::SetObjectsRenderedText(std::string const& text)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("ObjectsRendered"))
    {
        textComponent->SetText(text);
    }
}

void UserInterfaceScene::SetObjectsCulledText(std::string const& text)
{
    if (TextComponent* textComponent = GetComponentByGameObjectId<TextComponent>("ObjectsCulled"))
    {
        textComponent->SetText(text);
    }
}
