#pragma once

#include "Component.h"
#include <vector>

class MeshRendererComponent;

class LightComponent : public Component
{
    friend class MeshRendererComponent;

public:
    LightComponent(GameObject* owner);
    virtual ~LightComponent();

private:
    void InsertAffectedMeshRendererComponent(MeshRendererComponent* meshRendererComponent);
    void RemoveAffectedMeshRendererComponent(MeshRendererComponent* meshRendererComponent);

protected:
    std::vector<MeshRendererComponent*> affectedMeshRenderers;
};
