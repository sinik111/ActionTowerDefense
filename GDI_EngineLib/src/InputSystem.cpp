#include "pch.h"
#include "InputSystem.h"

InputSystem::InputSystem()
    :m_hWnd(nullptr)
{

}

void InputSystem::SetWindow(HWND hWnd)
{
    m_hWnd = hWnd;
}

void InputSystem::Update()
{
    m_PreviousKeyState = m_CurrentKeyState;

    for (int i = 0; i < 256; ++i)
    {
        m_CurrentKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
}

bool InputSystem::IsKeyDown(int vkey)
{
    return m_CurrentKeyState[vkey];
}

bool InputSystem::IsKeyPressed(int vkey)
{
    return !m_PreviousKeyState[vkey] && m_CurrentKeyState[vkey];
}

bool InputSystem::IsKeyReleased(int vkey)
{
    return m_PreviousKeyState[vkey] && !m_CurrentKeyState[vkey];
}

POINT InputSystem::GetCursorPosition()
{
    POINT point;

    GetCursorPos(&point);

    ScreenToClient(m_hWnd, &point);

    return point;
}
