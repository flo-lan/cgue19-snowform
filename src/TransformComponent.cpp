#include "TransformComponent.h"
#include "TransformGraphTraverser.h"
#include "GameObject.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>

static const glm::vec3 gAxisDirectionX = glm::vec3(1.f, 0.f, 0.f);
static const glm::vec3 gAxisDirectionY = glm::vec3(0.f, 1.f, 0.f);
static const glm::vec3 gAxisDirectionZ = glm::vec3(0.f, 0.f, 1.f);

TransformComponent::TransformComponent(GameObject* owner) :
    Component::Component(owner),
    parent(nullptr),
    localPosition(0.f, 0.f, 0.f),
    localRotation(0.f, 0.f, 0.f),
    localRotationQ(localRotation),
    localScale(1.f, 1.f, 1.f),
    position(0.f, 0.f, 0.f),
    rotation(0.f, 0.f, 0.f),
    rotationQ(rotation),
    scale(1.f, 1.f, 1.f),
    directionRight(rotationQ * gAxisDirectionX),
    directionUp(rotationQ * gAxisDirectionY),
    directionBackward(rotationQ * gAxisDirectionZ),
    modelMatrix(1.f),
    ignoreParentPosition(false),
    ignoreParentRotation(false),
    ignoreParentScale(false)
{
    GetOwner()->GetScene()->InsertSceneGraphRoot(this);
}

TransformComponent::~TransformComponent()
{
    if (parent)
    {
        parent->_RemoveChild(this);
    }
    else
    {
        GetOwner()->GetScene()->RemoveSceneGraphRoot(this);
    }

    std::vector<TransformComponent*> children_copy(children);

    for (std::vector<TransformComponent*>::const_iterator itr = children_copy.begin(); itr != children_copy.end(); ++itr)
    {
        RemoveChild(*itr);
    }
}

void TransformComponent::LateUpdate()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotationQ);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void TransformComponent::SetParent(TransformComponent* transform)
{
    if (transform == this)
    {
        return;
    }

    if (transform == parent)
    {
        return;
    }

    if (transform != nullptr)
    {
        transform->AddChild(this);
    }
    else if (parent != nullptr)
    {
        parent->RemoveChild(this);
    }
}

void TransformComponent::AddChild(TransformComponent* child)
{
    if (child == nullptr)
    {
        return;
    }

    if (child == this)
    {
        return;
    }

    if (child->GetParent())
    {
        child->GetParent()->_RemoveChild(child);
    }
    else
    {
        child->GetOwner()->GetScene()->RemoveSceneGraphRoot(child);
    }

    child->parent = this;

    _AddChild(child);

    // Update local position
    child->SetLocalPosition
    (
        position.x - child->GetPositionX(),
        position.y - child->GetPositionY(),
        position.z - child->GetPositionZ()
    );

    // Update local rotation
    child->SetLocalRotation(
        rotation.x - child->GetRotationX(),
        rotation.y - child->GetRotationY(),
        rotation.z - child->GetRotationZ()
    );

    // Update local scale
    // TODO
    //child->SetLocalScale(
    //    scale.x - child->GetScaleX(),
    //    scale.y - child->GetScaleY(),
    //    scale.z - child->GetScaleZ()
    //);
}

void TransformComponent::_AddChild(TransformComponent* child)
{
    children.push_back(child);
}

void TransformComponent::RemoveChild(TransformComponent* child)
{
    _RemoveChild(child);

    child->parent = nullptr;
    child->GetOwner()->GetScene()->InsertSceneGraphRoot(child);

    // Update local position
    child->SetLocalPosition(child->GetPositionX(), child->GetPositionY(), child->GetPositionZ());

    // Update local rotation
    child->SetLocalRotation(child->GetRotationX(), child->GetRotationY(), child->GetRotationZ());

    // Update local scale
    child->SetLocalScale(child->GetScaleX(), child->GetScaleY(), child->GetScaleZ());
}

void TransformComponent::_RemoveChild(TransformComponent* child)
{
    std::vector<TransformComponent*>::const_iterator itr = std::find(children.begin(), children.end(), child);

    if (itr == children.end())
    {
        return;
    }

    children.erase(itr);
}

void TransformComponent::TraverseTransformGraphDF(TransformGraphTraverser& traverser, bool traverseThis)
{
    std::stack<TransformComponent*> childStack;
    std::queue<TransformComponent*> childQueue;

    childStack.push(this);

    while (childStack.size())
    {
        TransformComponent* transform = childStack.top();

        childStack.pop();

        for (int32_t i = (int32_t)transform->GetChildCount() - 1; i >= 0; i--)
        {
            childStack.push(transform->GetChild(i));
        }

        childQueue.push(transform);
    }

    if (!traverseThis)
    {
        childQueue.pop();
    }

    while (childQueue.size())
    {
        traverser.Visit(childQueue.front());

        childQueue.pop();
    }
}

void TransformComponent::TraverseTransformGraphDFI(TransformGraphTraverser& traverser, bool traverseThis)
{
    std::stack<TransformComponent*> childStack;
    std::queue<TransformComponent*> childQueue;

    childQueue.push(this);

    while (childQueue.size())
    {
        TransformComponent* transform = childQueue.front();

        childQueue.pop();

        for (uint32_t i = 0; i < transform->GetChildCount(); i++)
        {
            childQueue.push(transform->GetChild(i));
        }

        childStack.push(transform);
    }

    size_t min = traverseThis ? 0 : 1;

    while (childStack.size() > min)
    {
        traverser.Visit(childStack.top());

        childStack.pop();
    }
}

void TransformComponent::SetLocalPositionX(float localPositionX)
{
    localPosition.x = localPositionX;

    if (parent != nullptr && !ignoreParentPosition)
    {
        position = parent->GetPosition() +
            directionRight    * localPosition.x +
            directionUp       * localPosition.y +
            directionBackward * localPosition.z;
    }
    else
    {
        position.x = localPosition.x;
    }

    TransformComponent* child = nullptr;
    for (uint32_t i = 0, size = children.size(); i < size; i++)
    {
        child = children[i];
        child->SetLocalPositionX(child->GetLocalPositionX()); // Update x position of child
    }
}

void TransformComponent::SetLocalPositionY(float localPositionY)
{
    localPosition.y = localPositionY;

    if (parent != nullptr && !ignoreParentPosition)
    {
        position = parent->GetPosition() +
            directionRight    * localPosition.x +
            directionUp       * localPosition.y +
            directionBackward * localPosition.z;
    }
    else
    {
        position.y = localPosition.y;
    }

    TransformComponent* child = nullptr;
    for (uint32_t i = 0, size = children.size(); i < size; i++)
    {
        child = children[i];
        child->SetLocalPositionY(child->GetLocalPositionY()); // Update y position of child
    }
}

void TransformComponent::SetLocalPositionZ(float localPositionZ)
{
    localPosition.z = localPositionZ;

    if (parent != nullptr && !ignoreParentPosition)
    {
        position = parent->GetPosition() +
            directionRight    * localPosition.x +
            directionUp       * localPosition.y +
            directionBackward * localPosition.z;
    }
    else
    {
        position.z = localPosition.z;
    }

    TransformComponent* child = nullptr;
    for (uint32_t i = 0, size = children.size(); i < size; i++)
    {
        child = children[i];
        child->SetLocalPositionZ(child->GetLocalPositionZ()); // Update z position of child
    }
}

void TransformComponent::SetLocalRotationX(float localRotationX)
{
    localRotation.x = localRotationX;
    localRotationQ = glm::quat(localRotation);

    if (parent != nullptr && !ignoreParentRotation)
    {
        rotation.x = parent->GetRotationX() + localRotation.x;
    }
    else
    {
        rotation.x = localRotation.x;
    }
    rotationQ = glm::quat(rotation);

    directionRight    = rotationQ * gAxisDirectionX;
    directionUp       = rotationQ * gAxisDirectionY;
    directionBackward = rotationQ * gAxisDirectionZ;

    // Recalculate position
    if (parent != nullptr && !ignoreParentPosition)
    {
        position = parent->GetPosition() +
            directionRight    * localPosition.x +
            directionUp       * localPosition.y +
            directionBackward * localPosition.z;
    }

    TransformComponent* child = nullptr;
    for (uint32_t i = 0, size = children.size(); i < size; i++)
    {
        child = children[i];
        child->SetLocalRotationX(child->GetLocalRotationX()); // Update x rotation of child
    }
}

void TransformComponent::SetLocalRotationY(float localRotationY)
{
    localRotation.y = localRotationY;
    localRotationQ = glm::quat(localRotation);

    if (parent != nullptr && !ignoreParentRotation)
    {
        rotation.y = parent->GetRotationY() + localRotation.y;
    }
    else
    {
        rotation.y = localRotation.y;
    }
    rotationQ = glm::quat(rotation);

    directionRight    = rotationQ * gAxisDirectionX;
    directionUp       = rotationQ * gAxisDirectionY;
    directionBackward = rotationQ * gAxisDirectionZ;

    // Recalculate position
    if (parent != nullptr && !ignoreParentPosition)
    {
        position = parent->GetPosition() +
            directionRight    * localPosition.x +
            directionUp       * localPosition.y +
            directionBackward * localPosition.z;
    }

    TransformComponent* child = nullptr;
    for (uint32_t i = 0, size = children.size(); i < size; i++)
    {
        child = children[i];
        child->SetLocalRotationY(child->GetLocalRotationY()); // Update y rotation of child
    }
}

void TransformComponent::SetLocalRotationZ(float localRotationZ)
{
    localRotation.z = localRotationZ;
    localRotationQ = glm::quat(localRotation);

    if (parent != nullptr && !ignoreParentRotation)
    {
        rotation.z = parent->GetRotationZ() + localRotation.z;
    }
    else
    {
        rotation.z = localRotation.z;
    }
    rotationQ = glm::quat(rotation);

    directionRight    = rotationQ * gAxisDirectionX;
    directionUp       = rotationQ * gAxisDirectionY;
    directionBackward = rotationQ * gAxisDirectionZ;

    // Recalculate position
    if (parent != nullptr && !ignoreParentPosition)
    {
        position = parent->GetPosition() +
            directionRight    * localPosition.x +
            directionUp       * localPosition.y +
            directionBackward * localPosition.z;
    }

    TransformComponent* child = nullptr;
    for (uint32_t i = 0, size = children.size(); i < size; i++)
    {
        child = children[i];
        child->SetLocalRotationZ(child->GetLocalRotationZ()); // Update z rotation of child
    }
}

void TransformComponent::SetLocalScaleX(float localScaleX)
{
    localScale.x = localScaleX;
    scale.x = localScale.x;

    // TODO
}

void TransformComponent::SetLocalScaleY(float localScaleY)
{
    localScale.y = localScaleY;
    scale.y = localScale.y;

    // TODO
}

void TransformComponent::SetLocalScaleZ(float localScaleZ)
{
    localScale.z = localScaleZ;
    scale.z = localScale.z;

    // TODO
}
