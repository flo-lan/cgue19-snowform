#pragma once

#include "RigidComponent.h"

namespace physx
{
    class PxRigidStatic;
}

class RigidStaticComponent : public RigidComponent
{
public:
    RigidStaticComponent(GameObject* owner);
    virtual ~RigidStaticComponent();

    virtual void SetGlobalPose(physx::PxTransform& pose);

private:
    physx::PxRigidStatic* pxRigidStatic;
};
