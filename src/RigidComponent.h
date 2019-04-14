#pragma once

#include "Component.h"

class GameObject;

class RigidComponent : public Component
{
protected:
    RigidComponent(GameObject* owner);

public:
    virtual ~RigidComponent();
};
