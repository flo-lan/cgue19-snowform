#pragma once

#include "RigidComponent.h"
#include "PxRigidBody.h"

class ColliderComponent;

namespace physx
{
    class PxRigidDynamic;
    class PxShape;
}

class RigidDynamicComponent : public RigidComponent
{
public:
    RigidDynamicComponent(GameObject* owner);
    virtual ~RigidDynamicComponent();

    physx::PxRigidBodyFlags GetPxRigidBodyFlags() const;
    void SetPxRigidBodyFlag(physx::PxRigidBodyFlag::Enum flag, bool value);

    virtual void SetGlobalPose(physx::PxTransform& globalPose);

protected:
    virtual void AttachShape(physx::PxShape* pxShape);
    virtual void DetachShape(physx::PxShape* pxShape);

private:
    physx::PxRigidDynamic* pxRigidDynamic;
};
