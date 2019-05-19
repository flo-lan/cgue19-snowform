#include "PointLightComponentFactory.h"
#include "PointLightComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "StringUtils.h"

PointLightComponentFactory::PointLightComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void PointLightComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    PointLightComponent* pointLight = gameObject->AttachComponent<PointLightComponent>();

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
                fprintf(stderr, "Could not parse color for point light component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        pointLight->SetColor(color);
    }

    if (element->Attribute("intensity"))
    {
        std::string intensityValue = std::string(element->Attribute("intensity"));
        pointLight->SetIntensity(std::strtof(intensityValue.c_str(), 0));
    }

    if (element->Attribute("constant"))
    {
        std::string constantValue = std::string(element->Attribute("constant"));
        pointLight->SetConstant(std::strtof(constantValue.c_str(), 0));
    }

    if (element->Attribute("linear"))
    {
        std::string linearValue = std::string(element->Attribute("linear"));
        pointLight->SetLinear(std::strtof(linearValue.c_str(), 0));
    }

    if (element->Attribute("quadratic"))
    {
        std::string quadraticValue = std::string(element->Attribute("quadratic"));
        pointLight->SetQuadratic(std::strtof(quadraticValue.c_str(), 0));
    }
}
