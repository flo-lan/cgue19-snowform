#pragma once

#include "Component.h"
#include "RigidDynamicComponent.h"

class GameScene;
class GameObject;
class TransformComponent;

class ThirdPersonControllerComponent : public Component
{
public:
    ThirdPersonControllerComponent(GameObject* owner);

    virtual void OnStart();
    virtual void Update();

private:
    GameScene* gameScene;
    TransformComponent* transform;
    TransformComponent* cameraTransform;
    RigidDynamicComponent* rigidDynamic;

    float velocity = 20.f;
    float jumpVelocity = 20.f;
    float lastMousePositionX;
    float lastMousePositionY;
    float lastMouseScrollValue;
};
