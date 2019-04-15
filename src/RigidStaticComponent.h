#pragma once

#include "RigidComponent.h"

class ColliderComponent;

namespace physx
{
    class PxRigidStatic;
    class PxShape;
}

class RigidStaticComponent : public RigidComponent
{
public:
    RigidStaticComponent(GameObject* owner);
    virtual ~RigidStaticComponent();

    virtual void SetGlobalPose(physx::PxTransform& globalPose);

protected:
    virtual void AttachShape(physx::PxShape* pxShape);
    virtual void DetachShape(physx::PxShape* pxShape);

private:
    physx::PxRigidStatic* pxRigidStatic;
};
