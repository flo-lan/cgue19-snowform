#pragma once

#include "RigidComponent.h"

class ColliderComponent;

namespace physx
{
    class PxRigidDynamic;
}

class RigidDynamicComponent : public RigidComponent
{
public:
    RigidDynamicComponent(GameObject* owner);
    virtual ~RigidDynamicComponent();

    virtual void AttachColliderComponent(ColliderComponent* collider);
    virtual void RemoveColliderComponent(ColliderComponent* collider);

    virtual void SetGlobalPose(physx::PxTransform& globalPose);

private:
    physx::PxRigidDynamic* pxRigidDynamic;
};
