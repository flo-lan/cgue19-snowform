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

    if (element->Attribute("shadowMapWidth"))
    {
        std::string shadowMapWidthValue = std::string(element->Attribute("shadowMapWidth"));
        directionalLight->SetShadowMapWidth(std::atoi(shadowMapWidthValue.c_str()));
    }

    if (element->Attribute("shadowMapHeight"))
    {
        std::string shadowMapHeightValue = std::string(element->Attribute("shadowMapHeight"));
        directionalLight->SetShadowMapHeight(std::atoi(shadowMapHeightValue.c_str()));
    }

    if (element->Attribute("shadowMapOrthoLeft"))
    {
        std::string shadowMapOrthoLeftValue = std::string(element->Attribute("shadowMapOrthoLeft"));
        directionalLight->SetShadowMapOrthoLeft(std::strtof(shadowMapOrthoLeftValue.c_str(), 0));
    }

    if (element->Attribute("shadowMapOrthoRight"))
    {
        std::string shadowMapOrthoRightValue = std::string(element->Attribute("shadowMapOrthoRight"));
        directionalLight->SetShadowMapOrthoRight(std::strtof(shadowMapOrthoRightValue.c_str(), 0));
    }

    if (element->Attribute("shadowMapOrthoBottom"))
    {
        std::string shadowMapOrthoBottomValue = std::string(element->Attribute("shadowMapOrthoBottom"));
        directionalLight->SetShadowMapOrthoBottom(std::strtof(shadowMapOrthoBottomValue.c_str(), 0));
    }

    if (element->Attribute("shadowMapOrthoTop"))
    {
        std::string shadowMapOrthoTopValue = std::string(element->Attribute("shadowMapOrthoTop"));
        directionalLight->SetShadowMapOrthoTop(std::strtof(shadowMapOrthoTopValue.c_str(), 0));
    }

    if (element->Attribute("shadowMapOrthoNear"))
    {
        std::string shadowMapOrthoNearValue = std::string(element->Attribute("shadowMapOrthoNear"));
        directionalLight->SetShadowMapOrthoNear(std::strtof(shadowMapOrthoNearValue.c_str(), 0));
    }

    if (element->Attribute("shadowMapOrthoFar"))
    {
        std::string shadowMapOrthoFarValue = std::string(element->Attribute("shadowMapOrthoFar"));
        directionalLight->SetShadowMapOrthoFar(std::strtof(shadowMapOrthoFarValue.c_str(), 0));
    }

    if (element->Attribute("shadow"))
    {
        directionalLight->EnableShadow(std::string(element->Attribute("shadow")) == "true");
    }
}
