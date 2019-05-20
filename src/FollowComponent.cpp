#include "FollowComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"

FollowComponent::FollowComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    targetId("")
{
}

void FollowComponent::Update()
{
    if (targetId == "")
    {
        return;
    }

    if (TransformComponent* targetTransform = GetOwner()->GetScene()->GetComponentByGameObjectId<TransformComponent>(targetId))
    {
        transform->SetPosition(targetTransform->GetPosition());
    }
}
