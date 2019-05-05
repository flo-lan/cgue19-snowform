#pragma once

#include "Component.h"
#include "CollectableComponent.h"

class TransformComponent;

class CoinComponent : public CollectableComponent
{
public:
    CoinComponent(GameObject* owner);

    virtual void OnTriggerEnter(ColliderComponent* other);
};
