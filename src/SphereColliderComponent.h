#pragma once

#include "ColliderComponent.h"

class SphereColliderComponent : public ColliderComponent
{
public:
    SphereColliderComponent(GameObject* owner);
    virtual ~SphereColliderComponent();

    float GetRadius() const { return radius; }
    void SetRadius(float value);

private:
    float radius;
};
