#include "SphereColliderComponent.h"
#include "GameObject.h"
#include "geometry/PxSphereGeometry.h"

SphereColliderComponent::SphereColliderComponent(GameObject* owner) :
    ColliderComponent::ColliderComponent(owner),
    radius(1.f)
{
    fprintf(stdout, "Attached sphere collider component to game object '%s'!\n", GetOwner()->GetName().c_str());

    SetPxGeometry(new physx::PxSphereGeometry(radius));
}

SphereColliderComponent::~SphereColliderComponent()
{
    fprintf(stdout, "Deleted sphere collider component from game object '%s'!\n", GetOwner()->GetName().c_str());
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
