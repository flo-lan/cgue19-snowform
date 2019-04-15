#pragma once

#include "RigidComponent.h"

class ColliderComponent;

namespace physx
{
    class PxRigidStatic;
}

class RigidStaticComponent : public RigidComponent
{
public:
    RigidStaticComponent(GameObject* owner);
    virtual ~RigidStaticComponent();

    virtual void AttachColliderComponent(ColliderComponent* collider);
    virtual void RemoveColliderComponent(ColliderComponent* collider);

    virtual void SetGlobalPose(physx::PxTransform& pose);

private:
    physx::PxRigidStatic* pxRigidStatic;
};
