#include "pch.h"
#include "MyTime.h"

#include "MyTimeSystem.h"

float MyTime::DeltaTime()
{
    return MyTimeSystem::Get().DeltaTime();
}

TimePoint MyTime::GetTimestamp()
{
    return Clock::now();
}

TimePoint MyTime::GetAccumulatedTime(const TimePoint& timePoint, int second)
{
    return timePoint + std::chrono::seconds(1);
}

float MyTime::GetElapsedTime(const TimePoint& timePoint)
{
    return Duration(Clock::now() - timePoint).count();
}
