#pragma once

#include <bitset>

#include "Singleton.h"

class InputSystem :
	public Singleton<InputSystem>
{
	friend class Singleton<InputSystem>;
private:
	HWND m_hWnd;
	std::bitset<256> m_CurrentKeyState;
	std::bitset<256> m_PreviousKeyState;
	POINT m_MousePoint;

private:
	InputSystem();
	~InputSystem() = default;

public:
	void SetWindow(HWND hWnd);

	// �� �������� ȣ�� �ʿ�
	void Update();

	// Ű�� ������ �������� Ȯ��
	bool IsKeyDown(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	bool IsKeyPressed(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	bool IsKeyReleased(int vkey);

	Vector2 GetCursorPosition();
};