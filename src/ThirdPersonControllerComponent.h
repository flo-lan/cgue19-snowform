#pragma once

#include "Component.h"
#include <string>

class GameObject;
class TransformComponent;
class RigidDynamicComponent;

class ThirdPersonControllerComponent : public Component
{
public:
    ThirdPersonControllerComponent(GameObject* owner);

    virtual void OnStart();
    virtual void Update();

    void SetTargetId(std::string const& targetId) { this->targetId = targetId; }
    void SetDistance(float distance) { this->distance = distance; }
    void SetMinDistance(float minDistance) { this->minDistance = minDistance; }
    void SetMaxDistance(float maxDistance) { this->maxDistance = maxDistance; }
    void SetScrollSpeed(float scrollSpeed) { this->scrollSpeed = scrollSpeed; }

private:
    TransformComponent* transform;
    std::string targetId;
    GameObject* target;
    TransformComponent* targetTransform;
    RigidDynamicComponent* targetRigid;
    float angle;
    float distance;
    float minDistance;
    float maxDistance;
    float scrollSpeed;
    float velocity;
    float jumpVelocity;
    float jumpTimer;
    float lastMousePositionX;
    float lastMousePositionY;
    float lastMouseScrollValue;
    float remainingMouseScroll;
};
