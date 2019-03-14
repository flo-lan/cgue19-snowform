#pragma once

#include "Component.h"

class GameObject;
class TransformComponent;

class ArcBallControllerComponent : public Component
{
public:
    ArcBallControllerComponent(GameObject* owner);

    virtual void OnStart();
    virtual void Update();

private:
    TransformComponent* transform;
    TransformComponent* cameraTransform;

    float lastMousePositionX;
    float lastMousePositionY;
    float lastMouseScrollValue;
};
