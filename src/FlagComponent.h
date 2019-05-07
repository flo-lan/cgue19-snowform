#pragma once

#include "Component.h"

class TransformComponent;

class FlagComponent : public Component
{
public:
    FlagComponent(GameObject* owner);

    virtual void OnStart();
    virtual void OnTriggerEnter(ColliderComponent* other);
    virtual void OnTriggerExit(ColliderComponent* other);

private:
    TransformComponent* transform;
};
