#pragma once

#include "Component.h"

class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* owner);
    virtual ~ParticleSystemComponent();
};
