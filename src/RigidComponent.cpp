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

void RigidComponent::OnStart()
{
    std::vector<ColliderComponent*> colliders;

    GetColliderComponents(colliders);

    for (auto itr = colliders.begin(); itr != colliders.end(); ++itr)
    {
        AttachColliderComponent(*itr);
    }
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

void RigidComponent::AttachColliderComponent(ColliderComponent* collider)
{
    if (!collider)
    {
        return;
    }

    physx::PxShape* pxShape = collider->GetPxShape();

    if (!pxShape)
    {
        return;
    }

    if (std::find(attachedColliders.begin(), attachedColliders.end(), collider) != attachedColliders.end())
    {
        return;
    }

    attachedColliders.push_back(collider);

    AttachShape(pxShape);
}

void RigidComponent::RemoveColliderComponent(ColliderComponent* collider)
{
    if (!collider)
    {
        return;
    }

    physx::PxShape* pxShape = collider->GetPxShape();

    if (!pxShape)
    {
        return;
    }

    auto itr = std::find(attachedColliders.begin(), attachedColliders.end(), collider);

    if (itr == attachedColliders.end())
    {
        return;
    }

    attachedColliders.erase(itr);

    DetachShape(pxShape);
}

void RigidComponent::LateUpdate()
{
    // ToDo: Update only if dirty
    SetGlobalPose(transform->GetPosition(), transform->GetRotationQ());
}

void RigidComponent::SetTransform(physx::PxTransform& globalPose)
{
    glm::vec3 eulerRotation = glm::eulerAngles
    (
        glm::quat(globalPose.q.x, globalPose.q.y, globalPose.q.z, globalPose.q.w)
    );

    transform->SetPosition(globalPose.p.x, globalPose.p.y, globalPose.p.z);
    transform->SetRotation(eulerRotation.x, eulerRotation.y, eulerRotation.z);
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
