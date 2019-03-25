#pragma once

#include "ComponentFactory.h"

class MeshRendererComponentFactory : public ComponentFactory
{
public:
    MeshRendererComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
