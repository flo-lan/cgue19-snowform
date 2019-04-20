#include "ThirdPersonControllerComponentFactory.h"
#include "ThirdPersonControllerComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

ThirdPersonControllerComponentFactory::ThirdPersonControllerComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void ThirdPersonControllerComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<ThirdPersonControllerComponent>();
}
