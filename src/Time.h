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

    void Pause() { paused = true; }
    float GetCountdownTime() const { return countdownTime; }
    float GetFPS() const { return fps; }

    float GetDelta() const;
    void Resume();
    void Update();
    void StartCountdown(float durationInMs);

private:
    Time();

    // difference since last update in ms
    float delta;
    float currentTime;
    float lastTime;

    float countdownTime;
    bool countdownRunning;

    float fps;
    float fpsLimit;
    // don't update delta time if paused
    bool paused;

    void UpdateDeltaTime();
};

#define sTime Time::getInstance()
