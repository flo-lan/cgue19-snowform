#include "DebugComponentFactory.h"
#include "DebugComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

DebugComponentFactory::DebugComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void DebugComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<DebugComponent>();
}
