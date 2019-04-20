#pragma once

#include "ComponentFactory.h"

class ThirdPersonControllerComponentFactory : public ComponentFactory
{
public:
    ThirdPersonControllerComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};

