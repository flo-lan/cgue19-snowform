#pragma once

#include "ComponentFactory.h"

class SceneOptionComponentFactory : public ComponentFactory
{
public:
    SceneOptionComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
