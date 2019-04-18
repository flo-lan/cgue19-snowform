#pragma once

#include "ComponentFactory.h"

class CameraComponentFactory : public ComponentFactory
{
public:
    CameraComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};

