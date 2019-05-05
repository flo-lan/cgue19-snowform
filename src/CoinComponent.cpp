#include "CoinComponent.h"
#include "GameObject.h"

CoinComponent::CoinComponent(GameObject* owner) :
    CollectableComponent::CollectableComponent(owner)
{
}

void CoinComponent::OnTriggerEnter(ColliderComponent* other)
{
    CollectableComponent::OnTriggerEnter(other);
}
