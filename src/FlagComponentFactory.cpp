#include "FlagComponentFactory.h"
#include "FlagComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "GameScene.h"

FlagComponentFactory::FlagComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void FlagComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<FlagComponent>();
}
