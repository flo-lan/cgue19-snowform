#include "UserInterfaceScene.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <GL\glew.h>
#include "SceneManager.h"
#include "GameScene.h"

UserInterfaceScene::UserInterfaceScene() :
    Scene::Scene("User Interface Scene")
{
}

UserInterfaceScene::~UserInterfaceScene()
{
}

void UserInterfaceScene::OnLoad()
{
    if (GameObject* countdown = GetGameObjectById("RemainingTime"))
    {
        if (TextComponent* textComponent = countdown->GetComponent<TextComponent>())
        {
            std::string countdownText = textComponent->GetText();
            std::string minutes = countdownText.substr(0, 2);
            std::string seconds = countdownText.substr(3, 2);
            sTime.StartCountdown(std::stoi(minutes) * 60 + std::stoi(seconds));
        }
    }
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

    if (GameObject* countdown = GetGameObjectById("RemainingTime"))
    {
        if (TextComponent* textComponent = countdown->GetComponent<TextComponent>())
        {
            float secondsLeft = sTime.GetCountdownTime();
            int minutesLeft = (int)(secondsLeft / 60.f);
            secondsLeft = (int)(secondsLeft - (60 * minutesLeft));

            std::string minutesLeftString = minutesLeft < 10 ? "0" + std::to_string(minutesLeft) : std::to_string(minutesLeft);
            std::string secondsLeftString = secondsLeft < 10 ? "0" + std::to_string((int)secondsLeft) : std::to_string((int)secondsLeft);
            std::string timeLeft = minutesLeftString + ":" + secondsLeftString;
            textComponent->SetText(timeLeft);
            if (secondsLeft == 0.f)
            {
                sSceneManager.GetScene<GameScene>()->OnGameLost();
            }
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
