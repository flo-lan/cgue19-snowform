#include "CoinComponentFactory.h"
#include "CoinComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "GameScene.h"

CoinComponentFactory::CoinComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void CoinComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<CoinComponent>();
}
