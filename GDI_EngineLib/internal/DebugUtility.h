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
    // �ܼ� ����
    void CreateConsole();
    // �ܼ� ����
    void ReleaseConsole();

    // �޸� ���� üũ Ȱ��ȭ
    // ���α׷� ����� �ڵ����� ������ �޸� ��µ�
    void EnableLeakCheck();
    // �޸� ���� ���
    // ���ϴ� �������� ������ �޸� Ȯ�ο�
    void DumpLeaks();

    // Frame Count ����, �� ������ ȣ�� �ؾ���
    void UpdateFPS(bool display = false);
    // �ֱ� FPS
    int GetLastFPS();
    // �ֱ� FPS �ֿܼ� ���, �⺻������ UpdateFPS���� ȣ�� ����
    void DisplayFPS();

    void Log(const std::string& log);
    void Log(const std::wstring& log);
};