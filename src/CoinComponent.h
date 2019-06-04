#pragma once

#include "CollectableComponent.h"

class TransformComponent;

class CoinComponent : public CollectableComponent
{
public:
    CoinComponent(GameObject* owner);

    virtual void OnStart();
    virtual void OnTriggerEnter(ColliderComponent* other);
};
