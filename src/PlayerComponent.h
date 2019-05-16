#pragma once

#include "Component.h"

class TransformComponent;

class PlayerComponent : public Component
{
public:
    PlayerComponent(GameObject* owner);

    virtual void Update();

    bool IsDead() const { return dead; }

private:
    TransformComponent* transform;
    bool dead;
};
