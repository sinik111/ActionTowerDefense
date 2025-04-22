#pragma once

class Input
{
private:
	Input() = delete;

public:
	// 키가 눌려진 상태인지 확인
	static bool IsKeyDown(int vkey);

	// 이번 루프에 키가 눌려졌는지 확인
	static bool IsKeyPressed(int vkey);

	// 이번 루프에 키가 놓아졌는지 확인
	static bool IsKeyReleased(int vkey);

	static POINT GetCursorPosition();
};