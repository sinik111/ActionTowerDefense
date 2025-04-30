#pragma once

#include <chrono>

#include "Singleton.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

class MyTimeSystem :
    public Singleton<MyTimeSystem>
{
    friend class Singleton<MyTimeSystem>;
private:
    TimePoint m_PreviousTime;
    TimePoint m_CurrentTime;

    float m_DeltaTime;

private:
    MyTimeSystem();
    ~MyTimeSystem() = default;

public:
    void Update();

    float DeltaTime();
};