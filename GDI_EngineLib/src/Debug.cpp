#include "pch.h"
#include "Debug.h"

#include "DebugUtility.h"

void Debug::CreateConsole()
{
#ifdef _DEBUG
    DebugUtility::Get().CreateConsole();
#endif // _DEBUG
}

void Debug::ReleaseConsole()
{
#ifdef _DEBUG
    DebugUtility::Get().ReleaseConsole();
#endif // _DEBUG
}

void Debug::EnableLeakCheck()
{
#ifdef _DEBUG
    DebugUtility::Get().EnableLeakCheck();
#endif // _DEBUG
}

void Debug::DumpLeaks()
{
#ifdef _DEBUG
    DebugUtility::Get().DumpLeaks();
#endif // _DEBUG
}

void Debug::UpdateFPS(bool display)
{
    DebugUtility::Get().UpdateFPS(display);
}

int Debug::GetLastFPS()
{
    return DebugUtility::Get().GetLastFPS();
}

void Debug::DisplayFPS()
{
#ifdef _DEBUG
    DebugUtility::Get().DisplayFPS();
#endif // _DEBUG
}

void Debug::Log(const std::string& log)
{
#ifdef _DEBUG
    DebugUtility::Get().Log(log);
#endif // _DEBUG
}

void Debug::Log(const std::wstring& log)
{
#ifdef _DEBUG
    DebugUtility::Get().Log(log);
#endif // _DEBUG
}
