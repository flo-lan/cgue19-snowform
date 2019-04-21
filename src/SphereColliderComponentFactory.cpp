#include "SphereColliderComponentFactory.h"
#include "SphereColliderComponent.h"
#include "PhysicsEngine.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "PxMaterial.h"

SphereColliderComponentFactory::SphereColliderComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void SphereColliderComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    SphereColliderComponent* sphereCollider = gameObject->AttachComponent<SphereColliderComponent>();

    if (element->Attribute("physicsMaterial"))
    {
        std::string physicsMaterialName = std::string(element->Attribute("physicsMaterial"));

        if (physx::PxMaterial* pxMaterial = sPhysicsEngine.GetPxMaterial(physicsMaterialName))
        {
            sphereCollider->SetPxMaterial(pxMaterial);
        }
        else
        {
            fprintf(stderr, "Could not find PhysX material '%s' for sphere collider component of game object '%s'!\n", physicsMaterialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("offset"))
    {
        std::string offsetString = std::string(element->Attribute("offset"));
        std::vector<std::string> offsetStringSplit = split(offsetString, ' ');

        glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f);

        switch (offsetStringSplit.size())
        {
        case 3:
            offset.z = (float)::atof(offsetStringSplit[2].c_str());
            // No break
        case 2:
            offset.y = (float)::atof(offsetStringSplit[1].c_str());
            // No break
        case 1:
            offset.x = (float)::atof(offsetStringSplit[0].c_str());
            break;
        default:
            fprintf(stderr, "Could not parse offset for sphere collider component of game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }

        sphereCollider->SetOffset(offset);
    }

    if (element->Attribute("radius"))
    {
        std::string radiusValue = std::string(element->Attribute("radius"));
        sphereCollider->SetRadius(std::strtof(radiusValue.c_str(), 0));
    }

    if (element->Attribute("trigger"))
    {
        sphereCollider->SetTrigger(std::string(element->Attribute("trigger")) == "true");
    }
}
