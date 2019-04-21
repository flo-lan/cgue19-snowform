#pragma once

#include "RigidComponent.h"
#include "PxRigidBody.h"
#include "PxForceMode.h"

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

    bool IsCCDEnabled() const { return GetPxRigidBodyFlags() & physx::PxRigidBodyFlag::eENABLE_CCD; }
    void EnableCCD(bool value) { SetPxRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, value); }

    void SetMass(float mass);
    void SetMassSpaceInertiaTensor(glm::vec3 const& m);

    void SetLinearVelocity(glm::vec3 const& linearVelocity);
    void SetAngularVelocity(glm::vec3 const& angularVelocity);

    void AddForce(glm::vec3 const& force, physx::PxForceMode::Enum mode = physx::PxForceMode::eFORCE);
    void AddTorque(glm::vec3 const& torque, physx::PxForceMode::Enum mode = physx::PxForceMode::eFORCE);

    virtual void SetGlobalPose(physx::PxTransform& globalPose);

protected:
    virtual void AttachShape(physx::PxShape* pxShape);
    virtual void DetachShape(physx::PxShape* pxShape);

private:
    physx::PxRigidDynamic* pxRigidDynamic;
};
