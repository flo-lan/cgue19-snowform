#pragma once

#include "Component.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

class GameObject;
class TransformComponent;
class ColliderComponent;

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

    virtual void OnAttachComponent(Component* component);
    virtual void OnRemoveComponent(Component* component);

    virtual void AttachColliderComponent(ColliderComponent* collider) = 0;
    virtual void RemoveColliderComponent(ColliderComponent* collider) = 0;

    virtual void LateUpdate();

    void SetTransform(physx::PxTransform& globalPose);
    void SetGlobalPose(glm::vec3 const& position, glm::quat const& rotation);
    virtual void SetGlobalPose(physx::PxTransform& globalPose) = 0;

protected:
    void GetColliderComponents(std::vector<ColliderComponent*>& colliders);

    TransformComponent* transform;
};
