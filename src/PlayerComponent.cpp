#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "UserInterfaceScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "GameObject.h"

PlayerComponent::PlayerComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    dead(false)
{
}

void PlayerComponent::Update()
{
    if (!dead && transform->GetPosition().y < -5.f)
    {
        dead = true;

        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->EnableFallenToDeathText(true);
        }

        if (auto gameScene = sSceneManager.GetScene<GameScene>())
        {
            gameScene->RestartLevel();
        }
    }
}
