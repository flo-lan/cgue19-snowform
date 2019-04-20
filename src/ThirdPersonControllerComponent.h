#pragma once

#include "Component.h"
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class GameObject;
class TransformComponent;

class ThirdPersonControllerComponent : public Component
{
public:
    ThirdPersonControllerComponent(GameObject* owner);

    virtual void OnStart();
    virtual void Update();

private:
    TransformComponent* transform;
    TransformComponent* cameraTransform;

    float lastMousePositionX;
    float lastMousePositionY;
    float lastMouseScrollValue;
};
