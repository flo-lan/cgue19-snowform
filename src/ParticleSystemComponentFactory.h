#pragma once

#include "ComponentFactory.h"

class ParticleSystemComponentFactory : public ComponentFactory
{
public:
    ParticleSystemComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
