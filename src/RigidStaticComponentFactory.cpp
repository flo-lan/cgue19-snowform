#include "RigidStaticComponentFactory.h"
#include "RigidStaticComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

RigidStaticComponentFactory::RigidStaticComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void RigidStaticComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<RigidStaticComponent>();
}
