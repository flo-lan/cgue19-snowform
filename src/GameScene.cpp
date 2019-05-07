#include "GameScene.h"
#include "UserInterfaceScene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Time.h"

GameScene::GameScene() :
    Scene::Scene("Game Scene"),
    coinCount(0),
    collectedCoinCount(0),
    remainingTimeInSecondsOption(0.f),
    remainingTimeInSeconds(0.f),
    transitionTimeInSecondsOption(0.f),
    transitionTimeInSeconds(0.f)
{
}

GameScene::~GameScene()
{
}

void GameScene::OnLoad()
{
    remainingTimeInSeconds = remainingTimeInSecondsOption;
}

void GameScene::OnUpdate()
{
    if (remainingTimeInSeconds > 0.f)
    {
        remainingTimeInSeconds -= sTime.GetDeltaTime();

        if (remainingTimeInSeconds <= 0.f)
        {
            remainingTimeInSeconds = 0.f;

            if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
            {
                userInterfaceScene->EnableTimeIsUpText(true);
            }

            RestartLevel();
        }

        if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            int minutesLeft = ((int)remainingTimeInSeconds) / 60;
            int secondsLeft = ((int)remainingTimeInSeconds) % 60;

            std::string minutesLeftString = minutesLeft < 10 ? "0" + std::to_string(minutesLeft) : std::to_string(minutesLeft);
            std::string secondsLeftString = secondsLeft < 10 ? "0" + std::to_string(secondsLeft) : std::to_string(secondsLeft);
            std::string timeLeft = minutesLeftString + ":" + secondsLeftString;
            userInterfaceScene->SetRemainingTimeText(timeLeft);
        }
    }

    if (transitionTimeInSeconds > 0.f)
    {
        transitionTimeInSeconds -= sTime.GetUnscaledDeltaTime();

        if (transitionTimeInSeconds < 0.f)
        {
            transitionTimeInSeconds = 0.f;

            // Restart or load next level
        }

        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->SetTransitionOverlayCutOff(transitionTimeInSeconds / transitionTimeInSecondsOption);
        }
    }
}

void GameScene::SetOption(std::string const& key, std::string const& value)
{
    if (key == "RemainingTime")
    {
        if (value.size() != 5)
        {
            fprintf(stderr, "Could not set game scene option '%s', because the value '%s' is invalid!\n", key.c_str(), value.c_str());
            return;
        }

        std::string minutes = value.substr(0, 2);
        std::string seconds = value.substr(3, 2);

        remainingTimeInSecondsOption = (float)(std::stoi(minutes) * 60 + std::stoi(seconds));
    }
    else if (key == "TransitionTime")
    {
        transitionTimeInSecondsOption = (float)std::atof(value.c_str());
    }
    else
    {
        fprintf(stderr, "Could not set unknown game scene option '%s'!\n", key.c_str());
        return;
    }

    fprintf(stdout, "Set game scene option '%s' with value '%s'!\n", key.c_str(), value.c_str());
}

void GameScene::CompleteLevel()
{
    fprintf(stdout, "Complete Level!\n");

    sTime.Pause();

    if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        userInterfaceScene->EnableTransitionOverlay(true);
        userInterfaceScene->SetTransitionOverlayCutOff(1.f);
    }

    transitionTimeInSeconds = transitionTimeInSecondsOption;
}

void GameScene::RestartLevel()
{
    fprintf(stdout, "Restart level!\n");

    sTime.Pause();

    if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        userInterfaceScene->EnableTransitionOverlay(true);
        userInterfaceScene->SetTransitionOverlayCutOff(1.f);
    }

    transitionTimeInSeconds = transitionTimeInSecondsOption;
}
