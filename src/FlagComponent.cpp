#include "GameObject.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "UserInterfaceScene.h"
#include "TextComponent.h"
#include "FlagComponent.h"
#include "ColliderComponent.h"

FlagComponent::FlagComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>())
{
}

void FlagComponent::OnStart()
{
}

void FlagComponent::OnTriggerEnter(ColliderComponent* other)
{
    if (other->GetOwner()->GetName() != "Snowball")
    {
        return;
    }

    if (auto gameScene = static_cast<GameScene*>(GetOwner()->GetScene()))
    {
        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            if (gameScene->GetCoinCount() == gameScene->GetCollectedCoinCount())
            {
                userInterfaceScene->EnableLevelFinishedText(true);

                gameScene->CompleteLevel();
            }
            else
            {
                userInterfaceScene->EnableCoinsLeftText(true);
            }
        }
    }
}

void FlagComponent::OnTriggerExit(ColliderComponent* other)
{
    if (other->GetOwner()->GetName() != "Snowball")
    {
        return;
    }

    if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        userInterfaceScene->EnableCoinsLeftText(false);
    }
}
