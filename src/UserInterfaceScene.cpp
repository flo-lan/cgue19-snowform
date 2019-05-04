#include "UserInterfaceScene.h"
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
