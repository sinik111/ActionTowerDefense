#include "pch.h"
#include "DebugUtility.h"

#include <crtdbg.h>

#include "MyTime.h"

DebugUtility::DebugUtility()
    : m_LastTimestamp(Clock::now()), m_LastFPS(0), m_FrameCount(0), m_IsConsoleOn(false)
{
    
}

void DebugUtility::CreateConsole()
{
    if (!m_IsConsoleOn)
    {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        SetConsoleTitleW(L"Debug Console");
        m_IsConsoleOn = true;

        Log("Console created");
    }
    else
    {
        Log("Console is already created");
    }

}

void DebugUtility::ReleaseConsole()
{
    if (m_IsConsoleOn)
    {
        // 표준 출력 스트림 닫기
        fclose(stdout);
        // 콘솔 해제
        FreeConsole();

        m_IsConsoleOn = false;

        Log("Console released");
    }
    else
    {
        Log("Console is not created");
    }
}

void DebugUtility::EnableLeakCheck()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void DebugUtility::DumpLeaks()
{
    _CrtDumpMemoryLeaks();
}

void DebugUtility::UpdateFPS(bool display)
{
    ++m_FrameCount;

    if (MyTime::GetElapsedSeconds(m_LastTimestamp) > 1.0f)
    {
        m_LastTimestamp = MyTime::GetAccumulatedTime(m_LastTimestamp, 1);

        m_LastFPS = m_FrameCount;

        m_FrameCount = 0;

#ifdef _DEBUG
        if (display)
        {
            DisplayFPS();
        }
#endif // _DEBUG
    }
}

int DebugUtility::GetLastFPS()
{
    return m_LastFPS;
}

void DebugUtility::DisplayFPS()
{
    Log(std::string("FPS: " + std::to_string(m_LastFPS)));
}

void DebugUtility::Log(const std::string& log)
{
    std::string str = log + "\n";

    OutputDebugStringA(str.c_str());

    if (m_IsConsoleOn)
    {
        std::cout << str;
    }
}

void DebugUtility::Log(const std::wstring& log)
{
    std::wstring str = log + L"\n";

    OutputDebugStringW(str.c_str());

    if (m_IsConsoleOn)
    {
        std::wcout << str;
    }
}
