#include "RigidDynamicComponentFactory.h"
#include "RigidDynamicComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include <algorithm>

RigidDynamicComponentFactory::RigidDynamicComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void RigidDynamicComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    RigidDynamicComponent* rigidDynamic = gameObject->AttachComponent<RigidDynamicComponent>();

    if (element->Attribute("kinematic"))
    {
        std::string kinematicValue = std::string(element->Attribute("kinematic"));
        std::transform(kinematicValue.begin(), kinematicValue.end(), kinematicValue.begin(), ::tolower);
        rigidDynamic->SetPxRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, kinematicValue == "true");
    }
}
