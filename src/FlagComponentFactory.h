#pragma once

#include "ComponentFactory.h"

class FlagComponentFactory : public ComponentFactory
{
public:
    FlagComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
