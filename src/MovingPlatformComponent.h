#pragma once

#include "Component.h"
#include <glm\glm.hpp>

class TransformComponent;

class MovingPlatformComponent : public Component
{
public:
    MovingPlatformComponent(GameObject* owner);

    virtual void OnStart();
    virtual void Update();

    void SetMoveDirection(glm::vec3 const& value) { moveDirection = value; }
    void SetLength(float value) { length = value; }
    void SetTime(float value) { time = value; }

private:
    TransformComponent* transform;
    glm::vec3 startLocalPosition;
    glm::vec3 moveDirection;
    float length;
    float time;
    float timer;
    float progress;
    bool forward;
};
