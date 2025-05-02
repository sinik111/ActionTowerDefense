#pragma once

#include "WinApp.h"
#include "ResultCode.h"

class GameApp :
	public WinApp
{
public:
	GameApp() = default;
	~GameApp() = default;

	ResultCode Initialize();
	void Shutdown() override;

	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};