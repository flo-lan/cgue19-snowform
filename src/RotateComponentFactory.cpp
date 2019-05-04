#include "RotateComponentFactory.h"
#include "RotateComponent.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include <glm\glm.hpp>

RotateComponentFactory::RotateComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void RotateComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    RotateComponent* rotateComponent = gameObject->AttachComponent<RotateComponent>();

    if (element->Attribute("rotationDirection"))
    {
        std::string rotationDirectionString = std::string(element->Attribute("rotationDirection"));
        std::vector<std::string> rotationDirectionStringSplit = split(rotationDirectionString, ' ');

        glm::vec3 rotationDirection = glm::vec3(0.f, 0.f, 0.f);

        switch (rotationDirectionStringSplit.size())
        {
        case 3:
            rotationDirection.z = (float)::atof(rotationDirectionStringSplit[2].c_str());
            // No break
        case 2:
            rotationDirection.y = (float)::atof(rotationDirectionStringSplit[1].c_str());
            // No break
        case 1:
            rotationDirection.x = (float)::atof(rotationDirectionStringSplit[0].c_str());
            break;
        default:
            fprintf(stderr, "Could not parse rotation direction for rotate component of game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }

        rotateComponent->SetRotationDirection(rotationDirection);
    }

    if (element->Attribute("speed"))
    {
        std::string speedValue = std::string(element->Attribute("speed"));
        rotateComponent->SetSpeed(std::strtof(speedValue.c_str(), 0));
    }
}
