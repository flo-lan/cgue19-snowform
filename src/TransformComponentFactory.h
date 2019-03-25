#pragma once

#include "ComponentFactory.h"

class TransformComponentFactory : public ComponentFactory
{
public:
    TransformComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
