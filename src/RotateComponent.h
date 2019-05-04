#pragma once

#include "Component.h"
#include <glm\glm.hpp>

class TransformComponent;

class RotateComponent : public Component
{
public:
    RotateComponent(GameObject* owner);

    virtual void Update();

    void SetRotationDirection(glm::vec3 value) { rotationDirection = value; }
    void SetSpeed(float value) { speed = value; }

private:
    TransformComponent* transform;
    glm::vec3 rotationDirection;
    float speed;
};
