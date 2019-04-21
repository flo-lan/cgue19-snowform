#pragma once

#include "ComponentFactory.h"

class MovingPlatformComponentFactory : public ComponentFactory
{
public:
    MovingPlatformComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
