#include "Time.h"
#include <GLFW\glfw3.h>
#include "Settings.h"
#include <windows.h>

Time::Time()
{
    unscaledDeltaTime = 0.f;
    deltaTime = 0.f;
    currentTime = glfwGetTime();
    lastTime = currentTime;

    countdownTime = 0.f;
    countdownRunning = false;

    fps = 0.f;
    fpsLimit = 1000.f / sSettings.getMaxFps(); // max ms per update
    timeScale = 1;
}

void Time::UpdateDeltaTime()
{
    // difference between delta and limit
    const float sleepTime = fpsLimit - ((glfwGetTime() - lastTime) * 1000);
    if (sleepTime > 0)
    {
        Sleep(sleepTime);
    }

    currentTime = glfwGetTime();
    unscaledDeltaTime = (currentTime - lastTime) * 1000;
    deltaTime = unscaledDeltaTime * timeScale;
    lastTime = currentTime;
}

void Time::Update()
{
    UpdateDeltaTime();

    fps = 1000 / unscaledDeltaTime;

    if (countdownRunning)
    {
        countdownTime -= deltaTime;
        if (countdownTime <= 0.f)
        {
            countdownRunning = false;
            countdownTime = 0.f;
        }
    }
}

void Time::StartCountdown(float durationInMs)
{
    // convert to seconds, glfw returns seconds
    countdownTime = durationInMs / 1000;
    countdownTime = true;
}