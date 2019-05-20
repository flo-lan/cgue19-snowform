#pragma once

#include "ComponentFactory.h"

class FollowComponentFactory : public ComponentFactory
{
public:
    FollowComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
