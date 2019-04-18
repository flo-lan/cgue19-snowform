#pragma once

#include "ComponentFactory.h"

class ArcBallControllerComponentFactory : public ComponentFactory
{
public:
    ArcBallControllerComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};

