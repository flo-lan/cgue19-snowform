#include "UserInterfaceScene.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <GL\glew.h>

UserInterfaceScene::UserInterfaceScene() :
    Scene::Scene("User Interface Scene")
{
}

UserInterfaceScene::~UserInterfaceScene()
{
}

void UserInterfaceScene::OnLoad()
{
}

void UserInterfaceScene::OnUpdate()
{
    if (GameObject* fps = GetGameObjectById("FPS"))
    {
        if (TextComponent* textComponent = fps->GetComponent<TextComponent>())
        {
            std::string fps = std::to_string((int)round(sTime.GetFPS()));
            textComponent->SetText(fps);
        }
    }
}

void UserInterfaceScene::OnPreRender()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UserInterfaceScene::OnPostRender()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
