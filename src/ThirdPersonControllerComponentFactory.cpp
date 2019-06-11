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

    if (element->Attribute("minDistance"))
    {
        std::string minDistanceValue = std::string(element->Attribute("minDistance"));
        thirdPersonController->SetMinDistance(std::strtof(minDistanceValue.c_str(), 0));
    }

    if (element->Attribute("maxDistance"))
    {
        std::string maxDistanceValue = std::string(element->Attribute("maxDistance"));
        thirdPersonController->SetMaxDistance(std::strtof(maxDistanceValue.c_str(), 0));
    }

    if (element->Attribute("scrollSpeed"))
    {
        std::string scrollSpeedValue = std::string(element->Attribute("scrollSpeed"));
        thirdPersonController->SetScrollSpeed(std::strtof(scrollSpeedValue.c_str(), 0));
    }
}
