#include "RotateComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Time.h"

RotateComponent::RotateComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    rotationDirection(0.f, 0.f, 0.f),
    speed(0.f)
{
}

void RotateComponent::Update()
{
    transform->Rotate(rotationDirection * speed * sTime.GetDeltaTime());
}
