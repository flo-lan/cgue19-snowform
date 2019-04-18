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
    fprintf(stdout, "Attached rigid dynamic component to game object '%s'!\n", GetOwner()->GetName().c_str());

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

    fprintf(stdout, "Deleted rigid dynamic component from game object '%s'!\n", GetOwner()->GetName().c_str());
}

physx::PxRigidBodyFlags RigidDynamicComponent::GetPxRigidBodyFlags() const
{
    return pxRigidDynamic ? pxRigidDynamic->getRigidBodyFlags() : (physx::PxRigidBodyFlag::Enum)0;
}

void RigidDynamicComponent::SetPxRigidBodyFlag(physx::PxRigidBodyFlag::Enum flag, bool value)
{
    if (pxRigidDynamic)
    {
        pxRigidDynamic->setRigidBodyFlag(flag, value);
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

    if (pxShape->getGeometryType() == physx::PxGeometryType::eTRIANGLEMESH &&
        pxRigidDynamic->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC) == false)
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
