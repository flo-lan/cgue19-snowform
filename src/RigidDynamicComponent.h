#pragma once

#include "RigidComponent.h"

namespace physx
{
    class PxRigidDynamic;
}

class RigidDynamicComponent : public RigidComponent
{
public:
    RigidDynamicComponent(GameObject* owner);
    virtual ~RigidDynamicComponent();

    virtual void SetGlobalPose(physx::PxTransform& pose);

private:
    physx::PxRigidDynamic* pxRigidDynamic;
};
