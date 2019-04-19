#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "geometry/PxBoxGeometry.h"

BoxColliderComponent::BoxColliderComponent(GameObject* owner) :
    ColliderComponent::ColliderComponent(owner),
    halfExtentX(1.f),
    halfExtentY(1.f),
    halfExtentZ(1.f)
{
    fprintf(stdout, "Attached box collider component to game object '%s'!\n", GetOwner()->GetName().c_str());

    SetPxGeometry(new physx::PxBoxGeometry(halfExtentX, halfExtentY, halfExtentZ));
}

BoxColliderComponent::~BoxColliderComponent()
{
    fprintf(stdout, "Deleted box collider component from game object '%s'!\n", GetOwner()->GetName().c_str());
}

void BoxColliderComponent::SetHalfExtent(float _halfExtentX, float _halfExtentY, float _halfExtentZ)
{
    if (_halfExtentX <= 0.f)
    {
        fprintf(stderr, "Could not set half extent x of box collider component of game object '%s', because it is <= 0!", GetOwner()->GetName().c_str());
        return;
    }

    if (_halfExtentY <= 0.f)
    {
        fprintf(stderr, "Could not set half extent y of box collider component of game object '%s', because it is <= 0!", GetOwner()->GetName().c_str());
        return;
    }

    if (_halfExtentZ <= 0.f)
    {
        fprintf(stderr, "Could not set half extent z of box collider component of game object '%s', because it is <= 0!", GetOwner()->GetName().c_str());
        return;
    }

    halfExtentX = _halfExtentX;
    halfExtentY = _halfExtentY;
    halfExtentZ = _halfExtentZ;

    SetPxGeometry(new physx::PxBoxGeometry(halfExtentX, halfExtentY, halfExtentZ));
}
