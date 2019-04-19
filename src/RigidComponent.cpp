#include "RigidComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "MeshColliderComponent.h"
#include "SphereColliderComponent.h"
#include "foundation/PxTransform.h"
#include "UniqueTypeId.h"

RigidComponent::RigidComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    lastPosition(owner->GetComponent<TransformComponent>()->GetPosition()),
    lastRotation(owner->GetComponent<TransformComponent>()->GetRotationQ())
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

    if (component->GetTypeId() == UniqueTypeId<BoxColliderComponent>() ||
        component->GetTypeId() == UniqueTypeId<MeshColliderComponent>() ||
        component->GetTypeId() == UniqueTypeId<SphereColliderComponent>())
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

    if (component->GetTypeId() == UniqueTypeId<BoxColliderComponent>() ||
        component->GetTypeId() == UniqueTypeId<MeshColliderComponent>() ||
        component->GetTypeId() == UniqueTypeId<SphereColliderComponent>())
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
    glm::vec3 positionDiff = lastPosition - transform->GetPosition();
    glm::quat rotationDiff = glm::quat
    (
        lastRotation.x - transform->GetRotationQ().x,
        lastRotation.y - transform->GetRotationQ().y,
        lastRotation.z - transform->GetRotationQ().z,
        lastRotation.w - transform->GetRotationQ().w
    );

    const float epsilon = 0.01f;

    if (positionDiff.x < -epsilon || positionDiff.x > epsilon ||
        positionDiff.y < -epsilon || positionDiff.y > epsilon ||
        positionDiff.z < -epsilon || positionDiff.z > epsilon ||
        rotationDiff.x < -epsilon || rotationDiff.x > epsilon ||
        rotationDiff.y < -epsilon || rotationDiff.y > epsilon ||
        rotationDiff.z < -epsilon || rotationDiff.z > epsilon ||
        rotationDiff.w < -epsilon || rotationDiff.w > epsilon)
    {
        SetGlobalPose(transform->GetPosition(), transform->GetRotationQ());
    }
}

ColliderComponent* RigidComponent::GetAttachedColliderByPxShape(physx::PxShape* pxShape) const
{
    for (auto itr = attachedColliders.begin(); itr != attachedColliders.end(); ++itr)
    {
        ColliderComponent* collider = *itr;

        if (collider->GetPxShape() == pxShape)
        {
            return collider;
        }
    }

    return nullptr;
}

void RigidComponent::SetTransform(physx::PxTransform& globalPose)
{
    lastPosition = glm::vec3(globalPose.p.x, globalPose.p.y, globalPose.p.z);
    lastRotation = glm::quat(globalPose.q.x, globalPose.q.y, globalPose.q.z, globalPose.q.w);

    transform->SetPosition(lastPosition);
    transform->SetRotation(lastRotation);
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
    std::vector<BoxColliderComponent*> boxColliders;
    std::vector<MeshColliderComponent*> meshColliders;
    std::vector<SphereColliderComponent*> sphereColliders;

    GetOwner()->GetComponents(boxColliders);
    GetOwner()->GetComponents(meshColliders);
    GetOwner()->GetComponents(sphereColliders);

    colliders.insert(colliders.end(), boxColliders.begin(), boxColliders.end());
    colliders.insert(colliders.end(), meshColliders.begin(), meshColliders.end());
    colliders.insert(colliders.end(), sphereColliders.begin(), sphereColliders.end());
}
