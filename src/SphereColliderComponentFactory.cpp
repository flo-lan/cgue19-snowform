#include "SphereColliderComponentFactory.h"
#include "SphereColliderComponent.h"
#include "PhysicsEngine.h"
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
