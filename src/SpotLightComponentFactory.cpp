#include "SpotLightComponentFactory.h"
#include "SpotLightComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "StringUtils.h"

SpotLightComponentFactory::SpotLightComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void SpotLightComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    SpotLightComponent* spotLight = gameObject->AttachComponent<SpotLightComponent>();

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
                fprintf(stderr, "Could not parse direction for spot light component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        spotLight->SetDirection(direction);
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
                fprintf(stderr, "Could not parse color for spot light component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        spotLight->SetColor(color);
    }

    if (element->Attribute("intensity"))
    {
        std::string intensityValue = std::string(element->Attribute("intensity"));
        spotLight->SetIntensity(std::strtof(intensityValue.c_str(), 0));
    }

    if (element->Attribute("innerCutoff"))
    {
        std::string innerCutoffValue = std::string(element->Attribute("innerCutoff"));
        spotLight->SetInnerCutOff(std::strtof(innerCutoffValue.c_str(), 0));
    }

    if (element->Attribute("outerCutoff"))
    {
        std::string outerCutoffValue = std::string(element->Attribute("outerCutoff"));
        spotLight->SetOuterCutOff(std::strtof(outerCutoffValue.c_str(), 0));
    }

    if (element->Attribute("constant"))
    {
        std::string constantValue = std::string(element->Attribute("constant"));
        spotLight->SetConstant(std::strtof(constantValue.c_str(), 0));
    }

    if (element->Attribute("linear"))
    {
        std::string linearValue = std::string(element->Attribute("linear"));
        spotLight->SetLinear(std::strtof(linearValue.c_str(), 0));
    }

    if (element->Attribute("quadratic"))
    {
        std::string quadraticValue = std::string(element->Attribute("quadratic"));
        spotLight->SetQuadratic(std::strtof(quadraticValue.c_str(), 0));
    }
}
