#include "RigidStaticComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "PhysicsEngine.h"
#include "PxRigidStatic.h"
#include "PxShape.h"

RigidStaticComponent::RigidStaticComponent(GameObject* owner) :
    RigidComponent::RigidComponent(owner),
    pxRigidStatic(nullptr)
{
    fprintf(stdout, "Attached rigid static component to game object '%s'!\n", GetOwner()->GetName().c_str());

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

    pxRigidStatic->userData = this;

    sPhysicsEngine.InsertRigidActor(pxRigidStatic);
}

RigidStaticComponent::~RigidStaticComponent()
{
    sPhysicsEngine.RemoveRigidActor(pxRigidStatic);

    if (pxRigidStatic)
    {
        pxRigidStatic->release();
        pxRigidStatic = nullptr;
    }

    fprintf(stdout, "Deleted rigid static component from game object '%s'!\n", GetOwner()->GetName().c_str());
}

void RigidStaticComponent::SetGlobalPose(physx::PxTransform& globalPose)
{
    if (pxRigidStatic)
    {
        pxRigidStatic->setGlobalPose(globalPose);
    }
}

void RigidStaticComponent::AttachShape(physx::PxShape* pxShape)
{
    if (!pxShape)
    {
        return;
    }

    if (!pxRigidStatic)
    {
        return;
    }

    pxRigidStatic->attachShape(*pxShape);
}

void RigidStaticComponent::DetachShape(physx::PxShape* pxShape)
{
    if (!pxShape)
    {
        return;
    }

    if (!pxRigidStatic)
    {
        return;
    }

    pxRigidStatic->detachShape(*pxShape);
}
