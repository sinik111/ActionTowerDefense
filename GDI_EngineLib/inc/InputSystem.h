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

	// 매 루프마다 호출 필요
	void Update();

	// 키가 눌려진 상태인지 확인
	bool IsKeyDown(int vkey);

	// 이번 루프에 키가 눌려졌는지 확인
	bool IsKeyPressed(int vkey);

	// 이번 루프에 키가 놓아졌는지 확인
	bool IsKeyReleased(int vkey);

	Vector2 GetCursorPosition();
};