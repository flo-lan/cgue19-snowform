#include "LightComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "GameObject.h"
#include <algorithm>
#include <assert.h>

LightComponent::LightComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>())
{
}

LightComponent::~LightComponent()
{
    assert(affectedMeshRenderers.size() == 0);
}

void LightComponent::InsertAffectedMeshRendererComponent(MeshRendererComponent* meshRendererComponent)
{
    if (std::find(affectedMeshRenderers.begin(), affectedMeshRenderers.end(), meshRendererComponent) != affectedMeshRenderers.end())
    {
        return;
    }

    affectedMeshRenderers.push_back(meshRendererComponent);
}

void LightComponent::RemoveAffectedMeshRendererComponent(MeshRendererComponent* meshRendererComponent)
{
    affectedMeshRenderers.erase(std::remove(affectedMeshRenderers.begin(), affectedMeshRenderers.end(), meshRendererComponent), affectedMeshRenderers.end());
}
