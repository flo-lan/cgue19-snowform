#include "PlayerComponentFactory.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "GameScene.h"

PlayerComponentFactory::PlayerComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void PlayerComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<PlayerComponent>();
}
