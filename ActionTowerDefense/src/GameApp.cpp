#include "framework.h"
#include "GameApp.h"
#include "resource.h"

#include "Debug.h"
#include "FileLoader.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "PlayScene.h"
#include "SoundManager.h"

ResultCode GameApp::Initialize()
{
	ResultCode rc = ResultCode::OK;

	HINSTANCE hInstance = GetModuleHandleW(NULL);

	m_ClassStyle = CS_HREDRAW | CS_VREDRAW;
	m_WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_hCursor = LoadCursorW(NULL, IDC_ARROW);
	m_hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_ACTIONTOWERDEFENSE));
	m_hIconSmall = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_SMALL));

	m_X = CW_USEDEFAULT;
	m_Y = CW_USEDEFAULT;
	m_Width = 1280;
	m_Height = 720;

	m_ClassName = L"gdi";
	m_WindowName = L"ActionTowerDefense";

	rc = __super::Initialize();
	if (rc != ResultCode::OK)
	{
		Debug::Log("Initialize fail GameApp::Initialize");

		return ResultCode::FAIL;
	}

	rc = FileLoader::Get().Initialize(m_ModulePath, L"bin", L"resource");
	if (rc != ResultCode::OK)
	{
		Debug::Log("FileLoader initialize fail - GameApp::Initialize");

		return ResultCode::FAIL;
	}

	std::string path = FileLoader::Get().GetPath();

	SoundManager::Get().CreateSound(path + "sound/PlayBGM.mp3", L"PlayBGM");
	SoundManager::Get().CreateSound(path + "sound/Lightning.mp3", L"Lightning");
	SoundManager::Get().CreateSound(path + "sound/Fireball.mp3", L"Fireball");
	SoundManager::Get().CreateSound(path + "sound/TitleBGM.mp3", L"TitleBGM");
	SoundManager::Get().CreateSound(path + "sound/Attack.mp3", L"Attack");

	SceneManager::Get().CreateScene<TitleScene>(L"TitleScene");
	SceneManager::Get().CreateScene<PlayScene>(L"PlayScene");

	SceneManager::Get().ChangeScene(L"TitleScene");

	m_IsRunning = true;

	return ResultCode::OK;
}

void GameApp::Shutdown()
{
	__super::Shutdown();
}

void GameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	__super::MessageProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			m_IsRunning = false;
		}
		break;
	}
}
