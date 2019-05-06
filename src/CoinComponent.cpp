#include "CoinComponent.h"
#include "GameObject.h"
#include "GameScene.h"

CoinComponent::CoinComponent(GameObject* owner) :
    CollectableComponent::CollectableComponent(owner)
{
}

void CoinComponent::OnTriggerEnter(ColliderComponent* other)
{
    auto gameScene = static_cast<GameScene*>(GetOwner()->GetScene());
    gameScene->SetCoinCount(gameScene->GetCoinCount() - 1);
    CollectableComponent::OnTriggerEnter(other);
}
