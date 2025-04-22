#pragma once

class Input
{
private:
	Input() = delete;

public:
	// Ű�� ������ �������� Ȯ��
	static bool IsKeyDown(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyPressed(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyReleased(int vkey);

	static POINT GetCursorPosition();
};