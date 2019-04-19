#include "BoxColliderComponentFactory.h"
#include "BoxColliderComponent.h"
#include "PhysicsEngine.h"
#include "GameObject.h"
#include "StringUtils.h"
#include "tinyxml2.h"

BoxColliderComponentFactory::BoxColliderComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void BoxColliderComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    BoxColliderComponent* boxCollider = gameObject->AttachComponent<BoxColliderComponent>();

    if (element->Attribute("physicsMaterial"))
    {
        std::string physicsMaterialName = std::string(element->Attribute("physicsMaterial"));

        if (physx::PxMaterial* pxMaterial = sPhysicsEngine.GetPxMaterial(physicsMaterialName))
        {
            boxCollider->SetPxMaterial(pxMaterial);
        }
        else
        {
            fprintf(stderr, "Could not find PhysX material '%s' for box collider component of game object '%s'!\n", physicsMaterialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("halfExtent"))
    {
        std::string halfExtentString = std::string(element->Attribute("halfExtent"));
        std::vector<std::string> halfExtentStringSplit = split(halfExtentString, ' ');

        float halfExtentX = 1.f;
        float halfExtentY = 1.f;
        float halfExtentZ = 1.f;

        switch (halfExtentStringSplit.size())
        {
            case 3:
                halfExtentZ = (float)::atof(halfExtentStringSplit[2].c_str());
                // No break
            case 2:
                halfExtentY = (float)::atof(halfExtentStringSplit[1].c_str());
                // No break
            case 1:
                halfExtentX = (float)::atof(halfExtentStringSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse half extent of box collider component for game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        boxCollider->SetHalfExtent(halfExtentX, halfExtentY, halfExtentZ);
    }

    if (element->Attribute("trigger"))
    {
        boxCollider->SetTrigger(std::string(element->Attribute("trigger")) == "true");
    }
}
