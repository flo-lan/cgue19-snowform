#include "CoinComponent.h"
#include "GameObject.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"

CoinComponent::CoinComponent(GameObject* owner) :
    CollectableComponent::CollectableComponent(owner)
{
}

void CoinComponent::OnStart()
{
    if (auto gameScene = static_cast<GameScene*>(GetOwner()->GetScene()))
    {
        gameScene->SetCoinCount(gameScene->GetCoinCount() + 1);
    }
}

void CoinComponent::OnTriggerEnter(ColliderComponent* other)
{
    if (!other->GetOwner()->GetComponent<PlayerComponent>())
    {
        return;
    }

    if (auto gameScene = static_cast<GameScene*>(GetOwner()->GetScene()))
    {
        gameScene->SetCollectedCoinCount(gameScene->GetCollectedCoinCount() + 1);
    }

    CollectableComponent::OnTriggerEnter(other);
}
