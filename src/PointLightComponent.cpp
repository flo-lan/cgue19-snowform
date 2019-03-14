#include "PointLightComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

PointLightComponent::PointLightComponent(GameObject* owner) :
    Component::Component(owner),
    color(1.f, 1.f, 1.f),
    intensity(1.f),
    constant(1.f),
    linear(1.f),
    quadratic(1.f)
{
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::OnStart()
{
    transform = GetOwner()->GetComponent<TransformComponent>();
}

glm::vec3 PointLightComponent::GetPosition() const
{
    return transform ? transform->GetPosition() : glm::vec3(0.f);
}
