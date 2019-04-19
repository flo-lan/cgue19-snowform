#include "Collision.h"
#include "ColliderComponent.h"
#include "RigidComponent.h"
#include "GameObject.h"

Collision::Collision(ColliderComponent* _collider, RigidComponent* _rigid) :
    collider(_collider),
    rigid(_rigid)
{
}

GameObject* Collision::GetGameObject() const
{
    if (collider)
    {
        return collider->GetOwner();
    }

    return nullptr;
}
