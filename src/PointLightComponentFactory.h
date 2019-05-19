#pragma once

#include "ComponentFactory.h"

class PointLightComponentFactory : public ComponentFactory
{
public:
    PointLightComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
