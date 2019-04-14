#include "RigidDynamicComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsEngine.h"
#include "PxRigidDynamic.h"

RigidDynamicComponent::RigidDynamicComponent(GameObject* owner) :
    RigidComponent::RigidComponent(owner),
    pxRigidDynamic(nullptr)
{
    if (transform)
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
    }
    else
    {
        pxRigidDynamic = sPhysicsEngine.CreatePxRigidDynamic(physx::PxTransform());
    }
}

RigidDynamicComponent::~RigidDynamicComponent()
{
    if (pxRigidDynamic)
    {
        pxRigidDynamic->release();
        pxRigidDynamic = nullptr;
    }
}

void RigidDynamicComponent::SetGlobalPose(physx::PxTransform& pose)
{
    if (pxRigidDynamic)
    {
        pxRigidDynamic->setGlobalPose(pose);
    }
}
