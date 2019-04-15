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

    pxRigidDynamic->userData = this;

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

void RigidDynamicComponent::SetGlobalPose(physx::PxTransform& globalPose)
{
    if (pxRigidDynamic)
    {
        pxRigidDynamic->setGlobalPose(globalPose);
    }
}

void RigidDynamicComponent::AttachShape(physx::PxShape* pxShape)
{
    if (!pxShape)
    {
        return;
    }

    if (!pxRigidDynamic)
    {
        return;
    }

    if (!pxRigidDynamic->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC))
    {
        if (pxShape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
        {
            fprintf(stderr, "Error: Could not attach PhysX shape to PhysX rigid dynamic of game object '%s', because attaching a triangle mesh shape configured as eSIMULATION_SHAPE is not supported for non-kinematic PhysX rigid dynamic instances!\n", GetOwner()->GetName().c_str());
            return;
        }
    }

    pxRigidDynamic->attachShape(*pxShape);
}

void RigidDynamicComponent::DetachShape(physx::PxShape* pxShape)
{
    if (!pxShape)
    {
        return;
    }

    if (!pxRigidDynamic)
    {
        return;
    }

    pxRigidDynamic->detachShape(*pxShape);
}
