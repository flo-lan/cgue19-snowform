#include "DirectionalLightComponent.h"
#include "GameObject.h"
#include "Scene.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* owner) :
    LightComponent::LightComponent(owner),
    color(1.f, 1.f, 1.f),
    direction(0.f, 0.f, 0.f),
    intensity(1.f)
{
    GetOwner()->GetScene()->InsertDirectionalLightComponent(this);
}

DirectionalLightComponent::~DirectionalLightComponent()
{
    while (affectedMeshRenderers.size())
    {
        affectedMeshRenderers[0]->RemoveLight(this);
    }

    GetOwner()->GetScene()->RemoveDirectionalLightComponent(this);
}
