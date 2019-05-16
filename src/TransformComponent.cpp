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

void TransformComponent::SetParent(TransformComponent* transform, bool keepGlobalChildTransformation)
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
        transform->AddChild(this, keepGlobalChildTransformation);
    }
    else if (parent != nullptr)
    {
        parent->RemoveChild(this, keepGlobalChildTransformation);
    }
}

void TransformComponent::AddChild(TransformComponent* child, bool keepGlobalChildTransformation)
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

    if (keepGlobalChildTransformation)
    {
        // Update local position
        child->SetLocalPosition
        (
            child->GetPositionX() - position.x,
            child->GetPositionY() - position.y,
            child->GetPositionZ() - position.z
        );

        // Update local rotation
        child->SetLocalRotation(
            child->GetRotationX() - rotation.x,
            child->GetRotationY() - rotation.y,
            child->GetRotationZ() - rotation.z
        );

        // Update local scale
        child->SetLocalScale(
            child->GetScaleX() / scale.x,
            child->GetScaleY() / scale.y,
            child->GetScaleZ() / scale.z
        );
    }
    else
    {
        UpdateRotation();
        UpdatePosition();
    }
}

void TransformComponent::_AddChild(TransformComponent* child)
{
    children.push_back(child);
}

void TransformComponent::RemoveChild(TransformComponent* child, bool keepGlobalChildTransformation)
{
    _RemoveChild(child);

    child->parent = nullptr;
    child->GetOwner()->GetScene()->InsertSceneGraphRoot(child);

    if (keepGlobalChildTransformation)
    {
        // Set local position to global position
        child->SetLocalPosition(child->GetPositionX(), child->GetPositionY(), child->GetPositionZ());

        // Set local rotation to global rotation
        child->SetLocalRotation(child->GetRotationX(), child->GetRotationY(), child->GetRotationZ());

        // Set local scale to global scale
        child->SetLocalScale(child->GetScaleX(), child->GetScaleY(), child->GetScaleZ());
    }
    else
    {
        UpdateRotation();
        UpdatePosition();
    }
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

void TransformComponent::UpdatePosition()
{
    if (parent != nullptr && !ignoreParentPosition)
    {
        if (!ignoreParentRotation)
        {
            position = parent->position +
                parent->directionRight    * localPosition.x +
                parent->directionUp       * localPosition.y +
                parent->directionBackward * localPosition.z;
        }
        else
        {
            position = parent->position + localPosition;
        }
    }
    else
    {
        position = localPosition;
    }
}

void TransformComponent::UpdateRotation()
{
    if (parent != nullptr && !ignoreParentRotation)
    {
        rotationQ = parent->rotationQ * localRotationQ;
        rotation = glm::eulerAngles(rotationQ);
    }
    else
    {
        rotationQ = localRotationQ;
        rotation = localRotation;
    }

    directionRight = rotationQ * gAxisDirectionX;
    directionUp = rotationQ * gAxisDirectionY;
    directionBackward = rotationQ * gAxisDirectionZ;
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

void TransformComponent::Rotate(glm::vec3 angles)
{
    localRotationQ = localRotationQ * glm::quat(angles);
    localRotation = glm::eulerAngles(localRotationQ);

    static struct UpdateChildTransformTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* child)
        {
            child->UpdateRotation();
            child->UpdatePosition();
        }
    } t;

    TraverseTransformGraphDF(t, true);
}

void TransformComponent::LookAt(glm::vec3 const& center, glm::vec3 const& worldUp)
{
    localRotationQ = glm::toQuat(glm::lookAt(position, center, worldUp));
    localRotation = glm::eulerAngles(localRotationQ);

    static struct UpdateChildTransformTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* child)
        {
            child->UpdateRotation();
            child->UpdatePosition();
        }
    } t;

    TraverseTransformGraphDF(t, true);
}

void TransformComponent::LookAt(TransformComponent* target, glm::vec3 const& worldUp)
{
    if (!target)
    {
        return;
    }

    LookAt(target->GetPosition(), worldUp);
}

void TransformComponent::SetLocalPositionX(float localPositionX)
{
    localPosition.x = localPositionX;

    UpdatePosition();

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

    UpdatePosition();

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

    UpdatePosition();

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

    UpdateRotation();
    UpdatePosition();

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

    UpdateRotation();
    UpdatePosition();

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

    UpdateRotation();
    UpdatePosition();

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

    if (parent != nullptr && !ignoreParentScale)
    {
        scale.x = parent->GetScaleX() * localScaleX;
    }
    else
    {
        scale.x = localScale.x;
    }

    static struct UpdateLocalScaleXTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* child)
        {
            child->SetLocalScaleX(child->GetLocalScaleX()); // Update x scale of child
        }
    } t;

    TraverseTransformGraphDF(t, false);
}

void TransformComponent::SetLocalScaleY(float localScaleY)
{
    localScale.y = localScaleY;

    if (parent != nullptr && !ignoreParentScale)
    {
        scale.y = parent->GetScaleY() * localScaleY;
    }
    else
    {
        scale.y = localScale.y;
    }

    static struct UpdateLocalScaleYTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* child)
        {
            child->SetLocalScaleY(child->GetLocalScaleY()); // Update y scale of child
        }
    } t;

    TraverseTransformGraphDF(t, false);
}

void TransformComponent::SetLocalScaleZ(float localScaleZ)
{
    localScale.z = localScaleZ;

    if (parent != nullptr && !ignoreParentScale)
    {
        scale.z = parent->GetScaleZ() * localScaleZ;
    }
    else
    {
        scale.z = localScale.z;
    }

    static struct UpdateLocalScaleZTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* child)
        {
            child->SetLocalScaleZ(child->GetLocalScaleZ()); // Update z scale of child
        }
    } t;

    TraverseTransformGraphDF(t, false);
}

void TransformComponent::SetPosition(glm::vec3 p)
{
    glm::vec3 localPositionOffset = glm::vec3
    (
        p.x - position.x,
        p.y - position.y,
        p.z - position.z
    );

    SetLocalPosition
    (
        localPosition.x + localPositionOffset.x,
        localPosition.y + localPositionOffset.y,
        localPosition.z + localPositionOffset.z
    );
}

void TransformComponent::SetRotation(glm::quat rq)
{
    if (parent != nullptr && !ignoreParentRotation)
    {
        localRotationQ = rq * glm::inverse(parent->rotationQ);
    }
    else
    {
        localRotationQ = rq;
    }

    localRotation = glm::eulerAngles(localRotationQ);

    static struct UpdateChildTransformTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* child)
        {
            child->UpdateRotation();
            child->UpdatePosition();
        }
    } t;

    TraverseTransformGraphDF(t, true);
}
