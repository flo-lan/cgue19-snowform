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
    class PxShape;
}

class RigidComponent : public Component
{
protected:
    RigidComponent(GameObject* owner);

public:
    virtual ~RigidComponent();

    virtual void OnStart();

    virtual void OnAttachComponent(Component* component);
    virtual void OnRemoveComponent(Component* component);

    void AttachColliderComponent(ColliderComponent* collider);
    void RemoveColliderComponent(ColliderComponent* collider);

    virtual void LateUpdate();

    ColliderComponent* GetAttachedColliderByPxShape(physx::PxShape* pxShape) const;

    void SetTransform(physx::PxTransform& globalPose);
    void SetGlobalPose(glm::vec3 const& position, glm::quat const& rotation);
    virtual void SetGlobalPose(physx::PxTransform& globalPose) = 0;

protected:
    void GetColliderComponents(std::vector<ColliderComponent*>& colliders);

    virtual void AttachShape(physx::PxShape* pxShape) = 0;
    virtual void DetachShape(physx::PxShape* pxShape) = 0;

    TransformComponent* transform;
    std::vector<ColliderComponent*> attachedColliders;

    glm::vec3 lastPosition;
    glm::quat lastRotation;
};
