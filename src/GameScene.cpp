#include "GameScene.h"
#include "UserInterfaceScene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Time.h"

GameScene::GameScene() :
    Scene::Scene("Game Scene"),
    gameState(GAME_STATE_INGAME_PLAYING),
    won(false),
    nextLevelScene(""),
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
    won = false;
    remainingTimeInSeconds = remainingTimeInSecondsOption;
}

void GameScene::OnUpdate()
{
    switch (gameState)
    {
        case GAME_STATE_INGAME_PLAYING:
        {
            UpdateRemainingTime();
            break;
        }
        case GAME_STATE_OVERLAY_TRANSITION:
        {
            UpdateOverlayTransition();
            break;
        }
        case GAME_STATE_OVERLAY_PAUSE:
        {
            if (sInputManager.IsAnyKeyPressed())
            {
                EnableOverlay(false);

                if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
                {
                    userInterfaceScene->DisableOverlayTexts();
                }
            }
            break;
        }
    }
}

void GameScene::UpdateRemainingTime()
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
}

void GameScene::UpdateOverlayTransition()
{
    if (transitionTimeInSeconds > 0.f)
    {
        transitionTimeInSeconds -= sTime.GetUnscaledDeltaTime();

        if (transitionTimeInSeconds <= 0.f)
        {
            transitionTimeInSeconds = 0.f;

            SetCoinCount(0);
            SetCollectedCoinCount(0);

            if (won) // Load next level
            {
                Unload();

                if (!nextLevelScene.empty())
                {
                    LoadFromFile(nextLevelScene);
                }
                else // Load credits scene
                {
                    // ToDo
                }
            }
            else // Restart level
            {
                Reload();
            }

            gameState = GAME_STATE_OVERLAY_PAUSE;
        }

        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->SetTransitionOverlayCutOff(transitionTimeInSeconds / transitionTimeInSecondsOption);
        }
    }
    else if (transitionTimeInSeconds < 0.f)
    {
        transitionTimeInSeconds += sTime.GetUnscaledDeltaTime();

        if (transitionTimeInSeconds >= 0.f)
        {
            transitionTimeInSeconds = 0.f;

            if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
            {
                userInterfaceScene->EnableTransitionOverlay(false);
            }

            gameState = GAME_STATE_INGAME_PLAYING;

            sTime.Resume();
        }

        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->SetTransitionOverlayCutOff((transitionTimeInSecondsOption + transitionTimeInSeconds) / transitionTimeInSecondsOption);
        }
    }
}

void GameScene::SetOption(std::string const& key, std::string const& value)
{
    if (key == "NextLevelScene")
    {
        nextLevelScene = value;
    }
    else if (key == "RemainingTime")
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

void GameScene::SetCoinCount(int coinCount)
{
    this->coinCount = coinCount;

    if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        std::string coinCountString = std::to_string(collectedCoinCount) + "/" + std::to_string(coinCount);
        userInterfaceScene->SetCoinCountText(coinCountString);
    }
}

void GameScene::SetCollectedCoinCount(int collectedCoinCount)
{
    this->collectedCoinCount = collectedCoinCount;

    if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        std::string coinCountString = std::to_string(collectedCoinCount) + "/" + std::to_string(coinCount);
        userInterfaceScene->SetCoinCountText(coinCountString);
    }
}

void GameScene::CompleteLevel()
{
    fprintf(stdout, "Complete Level!\n");

    won = true;

    sTime.Pause();

    EnableOverlay(true);
}

void GameScene::RestartLevel()
{
    fprintf(stdout, "Restart level!\n");

    sTime.Pause();

    EnableOverlay(true);
}

void GameScene::EnableOverlay(bool enable)
{
    gameState = GAME_STATE_OVERLAY_TRANSITION;

    if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        userInterfaceScene->EnableTransitionOverlay(true);
        userInterfaceScene->SetTransitionOverlayCutOff(enable ? 1.f : 0.f);
    }

    transitionTimeInSeconds = enable ? transitionTimeInSecondsOption : -transitionTimeInSecondsOption;
}
