#include "SphereColliderComponent.h"
#include "GameObject.h"
#include "geometry/PxSphereGeometry.h"

SphereColliderComponent::SphereColliderComponent(GameObject* owner) :
    ColliderComponent::ColliderComponent(owner),
    radius(1.f)
{
    SetPxGeometry(new physx::PxSphereGeometry(radius));
}

SphereColliderComponent::~SphereColliderComponent()
{
}

void SphereColliderComponent::SetRadius(float value)
{
    if (value < 0.f)
    {
        return;
    }

    if (value == radius)
    {
        return;
    }

    radius = value;

    SetPxGeometry(new physx::PxSphereGeometry(radius));
}
