#include "CoinComponent.h"
#include "GameObject.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "UserInterfaceScene.h"
#include "TextComponent.h"

CoinComponent::CoinComponent(GameObject* owner) :
    CollectableComponent::CollectableComponent(owner)
{
    auto gameScene = static_cast<GameScene*>(owner->GetScene());
    gameScene->SetCoinCount(gameScene->GetCoinCount() + 1);
}

void CoinComponent::OnTriggerEnter(ColliderComponent* other)
{
    auto gameScene = static_cast<GameScene*>(GetOwner()->GetScene());
    const int newCollectedCoinCount = gameScene->GetCollectedCoinCount() + 1;
    gameScene->SetCollectedCoinCount(newCollectedCoinCount);

    if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        if (GameObject* coinCount = userInterfaceScene->GetGameObjectById("CoinCount"))
        {
            if (TextComponent* textComponent = coinCount->GetComponent<TextComponent>())
            {
                std::string coinCountString = std::to_string(newCollectedCoinCount) + "/" + std::to_string(gameScene->GetCoinCount());
                textComponent->SetText(coinCountString);
            }
        }
    }
   
    CollectableComponent::OnTriggerEnter(other);
}
