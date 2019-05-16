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
    ThirdPersonControllerComponent* thirdPersonController = gameObject->AttachComponent<ThirdPersonControllerComponent>();

    if (element->Attribute("targetId"))
    {
        std::string targetId = std::string(element->Attribute("targetId"));
        thirdPersonController->SetTargetId(targetId);
    }

    if (element->Attribute("distance"))
    {
        std::string distanceValue = std::string(element->Attribute("distance"));
        thirdPersonController->SetDistance(std::strtof(distanceValue.c_str(), 0));
    }
}
