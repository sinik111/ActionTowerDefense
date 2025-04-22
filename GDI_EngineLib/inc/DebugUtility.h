#pragma once

#include <chrono>

#include "Singleton.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

class DebugUtility :
    public Singleton<DebugUtility>
{
    friend class Singleton<DebugUtility>;
private:
    TimePoint m_LastTimestamp;
    int m_LastFPS;
    int m_FrameCount;

    bool m_IsConsoleOn;

private:
    DebugUtility();
    ~DebugUtility() = default;

public:
    // 콘솔 생성
    void CreateConsole();
    // 콘솔 해제
    void ReleaseConsole();

    // 메모리 누수 체크 활성화
    // 프로그램 종료시 자동으로 누수된 메모리 출력됨
    void EnableLeakCheck();
    // 메모리 누수 출력
    // 원하는 시점에서 누수된 메모리 확인용
    void DumpLeaks();

    // Frame Count 누적, 매 프레임 호출 해야함
    void UpdateFPS(bool display = false);
    // 최근 FPS
    int GetLastFPS();
    // 최근 FPS 콘솔에 출력, 기본적으로 UpdateFPS에서 호출 중임
    void DisplayFPS();

    void Log(const std::string& log);
    void Log(const std::wstring& log);
};