#pragma once

#include "ComponentFactory.h"

class PlayerComponentFactory : public ComponentFactory
{
public:
    PlayerComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
