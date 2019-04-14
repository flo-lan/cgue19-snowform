#pragma once

#include "RigidComponent.h"

class RigidStaticComponent : public RigidComponent
{
public:
    RigidStaticComponent(GameObject* owner);
    virtual ~RigidStaticComponent();
};
