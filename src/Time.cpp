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

    fps = 0.f;
    fpsLimit = 1.f / sSettings.getMaxFps(); // max ms per update
    timeScale = 1;
}

void Time::UpdateDeltaTime()
{
    // difference between delta and limit
    const float sleepTime = fpsLimit - (glfwGetTime() - lastTime);
    if (sleepTime > 0)
    {
        Sleep(sleepTime * 1000); // time in milliseconds
    }

    currentTime = glfwGetTime();
    unscaledDeltaTime = currentTime - lastTime;
    deltaTime = unscaledDeltaTime * timeScale;
    lastTime = currentTime;
}

void Time::Update()
{
    UpdateDeltaTime();

    fps = 1.f / unscaledDeltaTime;
}
