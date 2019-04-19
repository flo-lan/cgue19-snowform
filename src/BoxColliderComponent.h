#pragma once

#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{
public:
    BoxColliderComponent(GameObject* owner);
    virtual ~BoxColliderComponent();

    void SetHalfExtent(float halfExtentX, float halfExtentY, float halfExtentZ);

private:
    float halfExtentX;
    float halfExtentY;
    float halfExtentZ;
};
