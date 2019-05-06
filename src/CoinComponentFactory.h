#pragma once

#include "ComponentFactory.h"

class CoinComponentFactory : public ComponentFactory
{
public:
    CoinComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element);
};
