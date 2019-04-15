#pragma once

#include "ComponentFactory.h"

class RigidDynamicComponentFactory : public ComponentFactory
{
public:
    RigidDynamicComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
