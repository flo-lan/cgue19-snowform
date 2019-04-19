#pragma once

#include "ComponentFactory.h"

class BoxColliderComponentFactory : public ComponentFactory
{
public:
    BoxColliderComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
