#include "RigidStaticComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsEngine.h"
#include "PxRigidStatic.h"

RigidStaticComponent::RigidStaticComponent(GameObject* owner) :
    RigidComponent::RigidComponent(owner),
    pxRigidStatic(nullptr)
{
    glm::vec3 position = transform->GetPosition();
    glm::quat rotation = transform->GetRotationQ();

    pxRigidStatic = sPhysicsEngine.CreatePxRigidStatic
    (
        physx::PxTransform
        (
            physx::PxVec3(position.x, position.y, position.z),
            physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)
        )
    );
}

RigidStaticComponent::~RigidStaticComponent()
{
    if (pxRigidStatic)
    {
        pxRigidStatic->release();
        pxRigidStatic = nullptr;
    }
}

void RigidStaticComponent::SetGlobalPose(physx::PxTransform& pose)
{
    if (pxRigidStatic)
    {
        pxRigidStatic->setGlobalPose(pose);
    }
}
