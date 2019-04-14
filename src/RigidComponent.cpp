#include "RigidComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "foundation/PxTransform.h"

RigidComponent::RigidComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>())
{
}

RigidComponent::~RigidComponent()
{
}

void RigidComponent::LateUpdate()
{
    if (transform)
    {
        // ToDo: Update only if dirty
        SetGlobalPose(transform->GetPosition(), transform->GetRotationQ());
    }
}

void RigidComponent::SetGlobalPose(glm::vec3& position, glm::quat& rotation)
{
    SetGlobalPose
    (
        physx::PxTransform
        (
            physx::PxVec3(position.x, position.y, position.z),
            physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)
        )
    );
}
