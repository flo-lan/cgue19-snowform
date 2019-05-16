#include "RigidStaticComponentFactory.h"
#include "RigidStaticComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

RigidStaticComponentFactory::RigidStaticComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void RigidStaticComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    RigidStaticComponent* rigidStatic = gameObject->AttachComponent<RigidStaticComponent>();

    if (element->Attribute("raycastLayerMask"))
    {
        std::string raycastLayerMaskValue = std::string(element->Attribute("raycastLayerMask"));
        rigidStatic->SetRaycastLayerMask(std::stoul(raycastLayerMaskValue.c_str()));
    }
}
