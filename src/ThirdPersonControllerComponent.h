#pragma once

#include "Component.h"
#include "RigidDynamicComponent.h"

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
    RigidDynamicComponent* rigidDynamic;

    float lastMousePositionX;
    float lastMousePositionY;
    float lastMouseScrollValue;
};
