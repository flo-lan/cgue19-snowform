#pragma once

#include "Component.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

class GameObject;
class TransformComponent;

namespace physx
{
    class PxTransform;
}

class RigidComponent : public Component
{
protected:
    RigidComponent(GameObject* owner);

public:
    virtual ~RigidComponent();

    virtual void LateUpdate();

    void SetGlobalPose(glm::vec3& position, glm::quat& rotation);
    virtual void SetGlobalPose(physx::PxTransform& pose) = 0;

protected:
    TransformComponent* transform;
};
