#pragma once

#include "Component.h"
#include <vector>

class TransformComponent;
class MeshRendererComponent;

class LightComponent : public Component
{
    friend class MeshRendererComponent;

protected:
    LightComponent(GameObject* owner);
    virtual ~LightComponent();

private:
    void InsertAffectedMeshRendererComponent(MeshRendererComponent* meshRendererComponent);
    void RemoveAffectedMeshRendererComponent(MeshRendererComponent* meshRendererComponent);

protected:
    TransformComponent* transform;
    std::vector<MeshRendererComponent*> affectedMeshRenderers;
};
