#pragma once

#include "ComponentFactory.h"

class DirectionalLightComponentFactory : public ComponentFactory
{
public:
    DirectionalLightComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
