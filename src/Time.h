#pragma once

class Time
{
public:
    static Time& getInstance()
    {
        static Time instance;
        return instance;
    }

    // We want to make sure these two methods are unacceptable,
    // otherwise we may accidentally get copies of our singleton
    Time(Time const&) = delete;
    void operator=(Time const&) = delete;

    void Pause() { timeScale = 0; }
    void Resume() { timeScale = 1; };
    float GetCountdownTime() const { return countdownTime; }
    float GetFPS() const { return fps; }
    float GetRealDeltaTime() { return realDeltaTime; }
    float GetDeltaTime() { return deltaTime; };;

    void Update();
    void StartCountdown(float durationInMs);

private:
    Time();

    // difference since last update in ms
    float realDeltaTime;
    // 0 if paused, otherwise realDeltaTime
    float deltaTime;
    float currentTime;
    float lastTime;

    float countdownTime;
    bool countdownRunning;

    float fps;
    float fpsLimit;
    // 0 if paused, 1 else
    // multiply to scaledDeltaTime to get pause effect
    int timeScale;

    void UpdateDeltaTime();
};

#define sTime Time::getInstance()
