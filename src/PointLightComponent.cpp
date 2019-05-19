#include "PointLightComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

PointLightComponent::PointLightComponent(GameObject* owner) :
    LightComponent::LightComponent(owner),
    color(1.f, 1.f, 1.f),
    intensity(1.f),
    constant(1.f),
    linear(1.f),
    quadratic(1.f)
{
}

PointLightComponent::~PointLightComponent()
{
    while (affectedMeshRenderers.size())
    {
        affectedMeshRenderers[0]->RemoveLight(this);
    }
}

glm::vec3 PointLightComponent::GetPosition() const
{
    return transform ? transform->GetPosition() : glm::vec3(0.f);
}
