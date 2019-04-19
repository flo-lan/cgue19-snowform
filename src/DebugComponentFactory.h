#pragma once

#include "ComponentFactory.h"

class DebugComponentFactory : public ComponentFactory
{
public:
    DebugComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
