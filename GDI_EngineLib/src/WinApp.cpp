#include "pch.h"
#include "WinApp.h"

#include "Debug.h"
#include "MyTime.h"
#include "Input.h"
#include "GDIRenderer.h"
#include "SceneManager.h"
#include "InputSystem.h"
#include "MyTimeSystem.h"
#include "RenderManager.h"

WinApp::WinApp()
	: m_hWnd(nullptr), m_hInstance(nullptr), m_Width(0), m_Height(0),
	m_ClassStyle(0), m_hIcon(nullptr), m_hCursor(nullptr), m_hIconSmall(nullptr),
	m_WindowStyle(0), m_X(0), m_Y(0), m_IsRunning(false)
{
	
}

// �����츦 �����Ҷ� ������ Ŭ���� �ν��Ͻ��� ���� �ٸ��� �ִ�.
// �ʱ� WM_NCCREATE ���� ������ �ڵ鿡 �ν��Ͻ� �ַθ� �����ϰ�
// �޼����� ������ ���� �������ڵ鿡 ������ �ּҷ� MessageProc �� ȣ���Ѵ�.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp* pWinApp = nullptr;
	// WM_NCCREATE: ������ ���� ���� �ʱ⿡, ������ ���� ����. WM_CREATE���� �������߻�
	if (uMsg == WM_NCCREATE)
	{
		// lParam�� CREATESTRUCT* �̴�
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWinApp = reinterpret_cast<WinApp*>(cs->lpCreateParams);

		// HWND�� this ������ ����
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWinApp));
	}
	else
	{
		// WM_NCCREATE�� �ƴ� ���� HWND���� this �����͸� �����´�
		pWinApp = reinterpret_cast<WinApp*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	if (pWinApp != nullptr)
	{
		pWinApp->MessageProc(hWnd, uMsg, wParam, lParam); // ��� �Լ� ȣ��
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void WinApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
}

ResultCode WinApp::Initialize()
{
	Debug::CreateConsole();

	wchar_t szPath[MAX_PATH] = {};

	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	m_ModulePath = szPath;

	GetCurrentDirectoryW(MAX_PATH, szPath);
	m_WorkingPath = szPath;

	OutputDebugStringW(std::wstring(m_ModulePath + L"\n").c_str());
	OutputDebugStringW(std::wstring(m_WorkingPath + L"\n").c_str());

	m_hInstance = GetModuleHandleW(NULL);

	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = m_ClassStyle;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_ClassName.c_str();
	wc.hCursor = m_hCursor;
	wc.hIcon = m_hIcon;
	wc.hCursor = m_hCursor;
	wc.hIconSm = m_hIconSmall;

	if (0 == RegisterClassExW(&wc))
	{
		Debug::Log("RegisterClassExW failed - WinApp::Initialize");

		return ResultCode::FAIL;
	}

	// ���ϴ� ũ�Ⱑ �����Ǿ� ����
	SIZE clientSize = { m_Width, m_Height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, m_WindowStyle, FALSE);

	m_hWnd = CreateWindowExW(
		0,
		m_ClassName.c_str(),
		m_WindowName.c_str(),
		m_WindowStyle,
		m_X, m_Y,
		clientRect.right - clientRect.left, // �ʺ�
		clientRect.bottom - clientRect.top, // ����
		NULL,
		NULL,
		m_hInstance,
		this // �ν��Ͻ� �ּҸ� NCREATESTRUCT�� lpCreateParams�� ����
	);

	if (m_hWnd == nullptr)
	{
		Debug::Log("CreateWindowExW failed - WinApp::Initialize");

		return ResultCode::FAIL;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	InputSystem::Get().SetWindow(m_hWnd);
	MyTimeSystem::Get().Update();
	GDIRenderer::Get().Initialize(m_hWnd, m_Width, m_Height);

	return ResultCode::OK;
}

void WinApp::Shutdown()
{
	SceneManager::Get().Shutdown();
	GDIRenderer::Get().Shutdown();
	Debug::ReleaseConsole();
}

void WinApp::Run()
{
	MSG msg = {};

	while (true)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		Update();
		Render();

		Debug::UpdateFPS(true);
	}
}

bool WinApp::IsRunning()
{
	return m_IsRunning;
}

void WinApp::Update()
{
	MyTimeSystem::Get().Update();

	InputSystem::Get().Update();

	SceneManager::Get().Update();
}

void WinApp::Render()
{
	RenderManager::Get().SortObjects();

	GDIRenderer::Get().BeginDraw();

	RenderManager::Get().RenderObjects();

	GDIRenderer::Get().EndDraw();

	RenderManager::Get().ClearObjects();
}
