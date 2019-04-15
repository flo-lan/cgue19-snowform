#include "RigidComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshColliderComponent.h"
#include "foundation/PxTransform.h"
#include "UniqueTypeId.h"

RigidComponent::RigidComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>())
{
}

RigidComponent::~RigidComponent()
{
}

void RigidComponent::OnAttachComponent(Component* component)
{
    if (!component)
    {
        return;
    }

    if (component->GetTypeId() == UniqueTypeId<MeshColliderComponent>())
    {
        AttachColliderComponent((ColliderComponent*)component);
    }
}

void RigidComponent::OnRemoveComponent(Component* component)
{
    if (!component)
    {
        return;
    }

    if (component->GetTypeId() == UniqueTypeId<MeshColliderComponent>())
    {
        RemoveColliderComponent((ColliderComponent*)component);
    }
}

void RigidComponent::LateUpdate()
{
    // ToDo: Update only if dirty
    SetGlobalPose(transform->GetPosition(), transform->GetRotationQ());
}

void RigidComponent::SetGlobalPose(glm::vec3 const& position, glm::quat const& rotation)
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

void RigidComponent::GetColliderComponents(std::vector<ColliderComponent*>& colliders)
{
    std::vector<MeshColliderComponent*> meshColliders;
    GetOwner()->GetComponents(meshColliders);

    colliders.insert(colliders.end(), meshColliders.begin(), meshColliders.end());
}
