#include "GameObject.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "UserInterfaceScene.h"
#include "TextComponent.h"
#include "FlagComponent.h"
#include "ColliderComponent.h"

FlagComponent::FlagComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    levelFinishedTextComponent(nullptr),
    coinsLeftTextComponent(nullptr)
{
}

void FlagComponent::OnStart()
{
    levelFinishedTextComponent = sSceneManager.GetScene<UserInterfaceScene>()->GetGameObjectById("LevelFinished")->GetComponent<TextComponent>();
    coinsLeftTextComponent = sSceneManager.GetScene<UserInterfaceScene>()->GetGameObjectById("CoinsLeft")->GetComponent<TextComponent>();
}

void FlagComponent::OnTriggerEnter(ColliderComponent* other)
{
    if (other->GetOwner()->GetName() != "Snowball") return;

    auto gameScene = static_cast<GameScene*>(GetOwner()->GetScene());
    const bool levelFinished = gameScene->GetCoinCount() == gameScene->GetCollectedCoinCount();

    if (levelFinished)
    {
        levelFinishedTextComponent->SetEnabled(true);
        gameScene->OnGameWon();
    }
    else
    {
        coinsLeftTextComponent->SetEnabled(true);
    }
}

void FlagComponent::OnTriggerExit(ColliderComponent* other)
{
    if (other->GetOwner()->GetName() != "Snowball") return;
    coinsLeftTextComponent->SetEnabled(false);
}