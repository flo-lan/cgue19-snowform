#include "TransformComponentFactory.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "StringUtils.h"
#include "tinyxml2.h"

TransformComponentFactory::TransformComponentFactory()
{
}

void TransformComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    TransformComponent* transform = gameObject->GetComponent<TransformComponent>();

    if (element->Attribute("localPosition"))
    {
        std::string localPositionString = std::string(element->Attribute("localPosition"));
        std::vector<std::string> localPositionStringSplit = split(localPositionString, ' ');

        switch (localPositionStringSplit.size())
        {
        case 3:
            transform->SetLocalPositionZ((float)::atof(localPositionStringSplit[2].c_str()));
            // No break
        case 2:
            transform->SetLocalPositionY((float)::atof(localPositionStringSplit[1].c_str()));
            // No break
        case 1:
            transform->SetLocalPositionX((float)::atof(localPositionStringSplit[0].c_str()));
            break;
        default:
            fprintf(stderr, "Could not parse local position for game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }
    }

    if (element->Attribute("localRotation"))
    {
        std::string localRotationString = std::string(element->Attribute("localRotation"));
        std::vector<std::string> localRotationStringSplit = split(localRotationString, ' ');

        switch (localRotationStringSplit.size())
        {
        case 3:
            transform->SetLocalRotationZ((float)::atof(localRotationStringSplit[2].c_str()));
            // No break
        case 2:
            transform->SetLocalRotationY((float)::atof(localRotationStringSplit[1].c_str()));
            // No break
        case 1:
            transform->SetLocalRotationX((float)::atof(localRotationStringSplit[0].c_str()));
            break;
        default:
            fprintf(stderr, "Could not parse local rotation for game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }
    }

    if (element->Attribute("localScale"))
    {
        std::string localScaleString = std::string(element->Attribute("localScale"));
        std::vector<std::string> localScaleStringSplit = split(localScaleString, ' ');

        switch (localScaleStringSplit.size())
        {
        case 3:
            transform->SetLocalScaleZ((float)::atof(localScaleStringSplit[2].c_str()));
            // No break
        case 2:
            transform->SetLocalScaleY((float)::atof(localScaleStringSplit[1].c_str()));
            // No break
        case 1:
            transform->SetLocalScaleX((float)::atof(localScaleStringSplit[0].c_str()));
            break;
        default:
            fprintf(stderr, "Could not parse local scale for game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }
    }
}
