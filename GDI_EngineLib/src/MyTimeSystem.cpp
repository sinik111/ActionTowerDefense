#include "pch.h"
#include "MyTimeSystem.h"

MyTimeSystem::MyTimeSystem()
    : m_PreviousTime(Clock::now()), m_CurrentTime(Clock::now()), m_DeltaTime(0.0f)
{

}

void MyTimeSystem::Update()
{
    m_CurrentTime = Clock::now();

    std::chrono::duration<float> duration(m_CurrentTime - m_PreviousTime);
    m_DeltaTime = duration.count();

    m_PreviousTime = m_CurrentTime;
}

float MyTimeSystem::DeltaTime()
{
    return m_DeltaTime;
}