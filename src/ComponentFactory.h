#pragma once

class GameObject;

namespace tinyxml2
{
    class XMLElement;
}

class ComponentFactory
{
public:
    ComponentFactory();
    virtual ~ComponentFactory();

    virtual void Build(GameObject* gameObject, tinyxml2::XMLElement* element) = 0;
};
