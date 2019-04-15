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

    std::vector<ColliderComponent*> colliders;

    GetColliderComponents(colliders);

    for (auto itr = colliders.begin(); itr != colliders.end(); ++itr)
    {
        AttachColliderComponent(*itr);
    }

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
}

void RigidStaticComponent::AttachColliderComponent(ColliderComponent* collider)
{
    if (!collider)
    {
        return;
    }

    if (!pxRigidStatic)
    {
        return;
    }

    physx::PxShape* pxShape = collider->GetPxShape();

    if (!pxShape)
    {
        return;
    }

    pxRigidStatic->attachShape(*pxShape);
}

void RigidStaticComponent::RemoveColliderComponent(ColliderComponent* collider)
{
    if (!collider)
    {
        return;
    }

    if (!pxRigidStatic)
    {
        return;
    }

    physx::PxShape* pxShape = collider->GetPxShape();

    if (!pxShape)
    {
        return;
    }

    pxRigidStatic->detachShape(*pxShape);
}

void RigidStaticComponent::SetGlobalPose(physx::PxTransform& globalPose)
{
    if (pxRigidStatic)
    {
        pxRigidStatic->setGlobalPose(globalPose);
    }
}
