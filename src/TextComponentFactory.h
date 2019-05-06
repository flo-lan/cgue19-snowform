#pragma once

#include "ComponentFactory.h"

class TextComponentFactory : public ComponentFactory
{
public:
    TextComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
