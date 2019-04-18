#pragma once

#include "ComponentFactory.h"

class MeshColliderComponentFactory : public ComponentFactory
{
public:
    MeshColliderComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
