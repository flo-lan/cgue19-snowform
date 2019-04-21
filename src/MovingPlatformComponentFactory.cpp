#include "MovingPlatformComponentFactory.h"
#include "MovingPlatformComponent.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include <glm\glm.hpp>

MovingPlatformComponentFactory::MovingPlatformComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void MovingPlatformComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    MovingPlatformComponent* movingPlatformComponent = gameObject->AttachComponent<MovingPlatformComponent>();

    if (element->Attribute("moveDirection"))
    {
        std::string moveDirectionString = std::string(element->Attribute("moveDirection"));
        std::vector<std::string> moveDirectionStringSplit = split(moveDirectionString, ' ');

        glm::vec3 moveDirection = glm::vec3(0.f, 0.f, 0.f);

        switch (moveDirectionStringSplit.size())
        {
            case 3:
                moveDirection.z = (float)::atof(moveDirectionStringSplit[2].c_str());
                // No break
            case 2:
                moveDirection.y = (float)::atof(moveDirectionStringSplit[1].c_str());
                // No break
            case 1:
                moveDirection.x = (float)::atof(moveDirectionStringSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse move direction for moving platform component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        movingPlatformComponent->SetMoveDirection(moveDirection);
    }

    if (element->Attribute("length"))
    {
        std::string lengthValue = std::string(element->Attribute("length"));
        movingPlatformComponent->SetLength(std::strtof(lengthValue.c_str(), 0));
    }

    if (element->Attribute("time"))
    {
        std::string timeValue = std::string(element->Attribute("time"));
        movingPlatformComponent->SetTime(std::strtof(timeValue.c_str(), 0));
    }
}
