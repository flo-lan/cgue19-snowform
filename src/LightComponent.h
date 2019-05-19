#pragma once

#include "Component.h"

class LightComponent : public Component
{
public:
    LightComponent(GameObject* owner);
    virtual ~LightComponent();
};
