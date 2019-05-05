#include "CollectableComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

CollectableComponent::CollectableComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>())
{
}

void CollectableComponent::OnTriggerEnter(ColliderComponent* other)
{

}
