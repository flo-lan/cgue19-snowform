#pragma once

#include "RigidComponent.h"

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

    virtual void SetGlobalPose(physx::PxTransform& globalPose);

protected:
    virtual void AttachShape(physx::PxShape* pxShape);
    virtual void DetachShape(physx::PxShape* pxShape);

private:
    physx::PxRigidDynamic* pxRigidDynamic;
};
