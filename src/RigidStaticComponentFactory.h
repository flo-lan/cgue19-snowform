#pragma once

#include "ComponentFactory.h"

class RigidStaticComponentFactory : public ComponentFactory
{
public:
    RigidStaticComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
