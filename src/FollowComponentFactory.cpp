#include "FollowComponentFactory.h"
#include "FollowComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

FollowComponentFactory::FollowComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void FollowComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    FollowComponent* followComponent = gameObject->AttachComponent<FollowComponent>();

    if (element->Attribute("targetId"))
    {
        std::string targetId = std::string(element->Attribute("targetId"));
        followComponent->SetTargetId(targetId);
    }
}
