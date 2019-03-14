#include "SpotLightComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

SpotLightComponent::SpotLightComponent(GameObject* owner) :
    Component::Component(owner),
    direction(0.f, 0.f, 0.f),
    color(1.f, 1.f, 1.f),
    intensity(1.f),
    innerCutOff(0.f),
    outerCutOff(0.f),
    constant(1.f),
    linear(1.f),
    quadratic(1.f)
{
}

SpotLightComponent::~SpotLightComponent()
{
}

void SpotLightComponent::OnStart()
{
    transform = GetOwner()->GetComponent<TransformComponent>();
}

glm::vec3 SpotLightComponent::GetPosition() const
{
    return transform ? transform->GetPosition() : glm::vec3(0.f);
}
