#pragma once

#include "ComponentFactory.h"

class SpotLightComponentFactory : public ComponentFactory
{
public:
    SpotLightComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
