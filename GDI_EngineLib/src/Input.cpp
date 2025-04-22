#include "pch.h"
#include "Input.h"

#include "InputSystem.h"

bool Input::IsKeyDown(int vkey)
{
	return InputSystem::Get().IsKeyDown(vkey);
}

bool Input::IsKeyPressed(int vkey)
{
	return InputSystem::Get().IsKeyPressed(vkey);
}

bool Input::IsKeyReleased(int vkey)
{
	return InputSystem::Get().IsKeyReleased(vkey);
}

POINT Input::GetCursorPosition()
{
	return InputSystem::Get().GetCursorPosition();
}
