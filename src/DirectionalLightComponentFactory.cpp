#include "DirectionalLightComponentFactory.h"
#include "DirectionalLightComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "StringUtils.h"

DirectionalLightComponentFactory::DirectionalLightComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void DirectionalLightComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    DirectionalLightComponent* directionalLight = gameObject->AttachComponent<DirectionalLightComponent>();

    if (element->Attribute("direction"))
    {
        std::string directionString = std::string(element->Attribute("direction"));
        std::vector<std::string> directionStringSplit = split(directionString, ' ');

        glm::vec3 direction = glm::vec3(0.f, 0.f, 0.f);

        switch (directionStringSplit.size())
        {
        case 3:
            direction.z = (float)::atof(directionStringSplit[2].c_str());
            // No break
        case 2:
            direction.y = (float)::atof(directionStringSplit[1].c_str());
            // No break
        case 1:
            direction.x = (float)::atof(directionStringSplit[0].c_str());
            break;
        default:
            fprintf(stderr, "Could not parse direction for directional light component of game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }

        directionalLight->SetDirection(direction);
    }

    if (element->Attribute("color"))
    {
        std::string colorString = std::string(element->Attribute("color"));
        std::vector<std::string> colorStringSplit = split(colorString, ' ');

        glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);

        switch (colorStringSplit.size())
        {
            case 3:
                color.z = (float)::atof(colorStringSplit[2].c_str());
                // No break
            case 2:
                color.y = (float)::atof(colorStringSplit[1].c_str());
                // No break
            case 1:
                color.x = (float)::atof(colorStringSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse color for directional light component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        directionalLight->SetColor(color);
    }

    if (element->Attribute("intensity"))
    {
        std::string intensityValue = std::string(element->Attribute("intensity"));
        directionalLight->SetIntensity(std::strtof(intensityValue.c_str(), 0));
    }
}
