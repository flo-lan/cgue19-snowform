#pragma once

#include "Component.h"

class TransformComponent;

class CollectableComponent : public Component
{
public:
    CollectableComponent(GameObject* owner);

    virtual void OnTriggerEnter(ColliderComponent* other);

private:
    TransformComponent* transform;
};
