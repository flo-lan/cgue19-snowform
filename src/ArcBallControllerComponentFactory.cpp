#include "ArcBallControllerComponentFactory.h"
#include "ArcBallControllerComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

ArcBallControllerComponentFactory::ArcBallControllerComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void ArcBallControllerComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<ArcBallControllerComponent>();
}
