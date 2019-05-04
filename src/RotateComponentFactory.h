#pragma once

#include "ComponentFactory.h"

class RotateComponentFactory : public ComponentFactory
{
public:
    RotateComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
