#include "RigidDynamicComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "PhysicsEngine.h"
#include "PxRigidDynamic.h"

RigidDynamicComponent::RigidDynamicComponent(GameObject* owner) :
    RigidComponent::RigidComponent(owner),
    pxRigidDynamic(nullptr)
{
    glm::vec3 position = transform->GetPosition();
    glm::quat rotation = transform->GetRotationQ();

    pxRigidDynamic = sPhysicsEngine.CreatePxRigidDynamic
    (
        physx::PxTransform
        (
            physx::PxVec3(position.x, position.y, position.z),
            physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)
        )
    );

    std::vector<ColliderComponent*> colliders;

    GetColliderComponents(colliders);

    for (auto itr = colliders.begin(); itr != colliders.end(); ++itr)
    {
        AttachColliderComponent(*itr);
    }

    sPhysicsEngine.InsertRigidActor(pxRigidDynamic);
}

RigidDynamicComponent::~RigidDynamicComponent()
{
    sPhysicsEngine.RemoveRigidActor(pxRigidDynamic);

    if (pxRigidDynamic)
    {
        pxRigidDynamic->release();
        pxRigidDynamic = nullptr;
    }
}

void RigidDynamicComponent::AttachColliderComponent(ColliderComponent* collider)
{
    if (!collider)
    {
        return;
    }

    if (!pxRigidDynamic)
    {
        return;
    }

    physx::PxShape* pxShape = collider->GetPxShape();

    if (!pxShape)
    {
        return;
    }

    pxRigidDynamic->attachShape(*pxShape);
}

void RigidDynamicComponent::RemoveColliderComponent(ColliderComponent* collider)
{
    if (!collider)
    {
        return;
    }

    if (!pxRigidDynamic)
    {
        return;
    }

    physx::PxShape* pxShape = collider->GetPxShape();

    if (!pxShape)
    {
        return;
    }

    pxRigidDynamic->detachShape(*pxShape);
}

void RigidDynamicComponent::SetGlobalPose(physx::PxTransform& pose)
{
    if (pxRigidDynamic)
    {
        pxRigidDynamic->setGlobalPose(pose);
    }
}
