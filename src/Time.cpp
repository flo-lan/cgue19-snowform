#include "Time.h"
#include <GLFW\glfw3.h>

Time::Time()
{
    delta = 0.f;
    currentTime = glfwGetTime();
    lastTime = currentTime;

    countdownTime = 0.f;
    countdownRunning = false;

    fps = 0.f;
    paused = false;
}

float Time::GetDelta() const
{
    return paused ? 0.f : delta;
}

void Time::Resume()
{
    lastTime = glfwGetTime();
}

void Time::Update()
{
    if (paused) return;

    currentTime = glfwGetTime();
    delta = (currentTime - lastTime) * 1000;
    lastTime = currentTime;

    fps = 1000 / delta;

    if (countdownRunning)
    {
        countdownTime -= delta;
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