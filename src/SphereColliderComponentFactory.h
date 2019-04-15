#pragma once

#include "ComponentFactory.h"

class SphereColliderComponentFactory : public ComponentFactory
{
public:
    SphereColliderComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
