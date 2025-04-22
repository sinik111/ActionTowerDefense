#pragma once

#include "ResultCode.h"

class WinApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	UINT m_ClassStyle;
	HICON m_hIcon;
	HCURSOR m_hCursor;
	HICON m_hIconSmall;
	DWORD m_WindowStyle;

	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;
	std::wstring m_ClassName;
	std::wstring m_WindowName;
	std::wstring m_ModulePath;
	std::wstring m_WorkingPath;

	bool m_IsRunning;

public:
	WinApp();
	virtual ~WinApp() = default;

public:
	virtual ResultCode Initialize();
	virtual void Shutdown();

	void Run();
	bool IsRunning();

private:
	void Update();
	void Render();

protected:
	virtual void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
};