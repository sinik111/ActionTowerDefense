#include "framework.h"
#include "TitleScene.h"

#include <sstream>

#include "ResultCode.h"
#include "Debug.h"
#include "ResourceManager.h"
#include "FileLoader.h"
#include "GDIRenderer.h"
#include "StaticBackground.h"
#include "ScreenTextUI.h"
#include "Input.h"
#include "SceneManager.h"
#include "SoundManager.h"

TitleScene::~TitleScene()
{
	Unload();
}

ResultCode TitleScene::Load()
{
	Debug::Log("Load Title Scene - TitleScene::Load");

	ResultCode rc = ResultCode::OK;

	rc = LoadResources();
	if (rc != ResultCode::OK)
	{
		return rc;
	}

	return ResultCode::OK;
}

void TitleScene::Enter()
{
	__super::Enter();

	Debug::Log("Enter Title Scene - TitleScene::Enter");

	SoundManager::Get().PlayBGM(L"TitleBGM", 0.1f);

	CreateObject<StaticBackground>(ResourceManager::Get().GetImage(L"Title", L"TitleImage"));

	CreateObject<ScreenTextUI>(L"게임을 시작하려면 Enter를 누르세요.", Vector2(430.0f, 500.0f), Gdiplus::Color(0, 0, 0), 24);
}

void TitleScene::Exit()
{
	Debug::Log("Exit Title Scene - TitleScene::Exit");

	SoundManager::Get().StopBGM();

	__super::Exit();
}

void TitleScene::Unload()
{
	Debug::Log("Unload Title Scene - TitleScene::Unload");

	ResourceManager::Get().ReleaseResources(L"Title");
}

void TitleScene::Update()
{
	if (Input::IsKeyReleased(VK_RETURN))
	{
		SceneManager::Get().ChangeScene(L"PlayScene");
	}

	__super::Update();
}

ResultCode TitleScene::LoadResources()
{
	ResultCode rc = ResultCode::OK;

	std::wstring data = FileLoader::Get().LoadTextFile(L"data/TitleSceneResourceList.txt");
	if (data == L"")
	{
		return ResultCode::FAIL;
	}

	rc = ResourceManager::Get().AddByData(data);
	if (rc != ResultCode::OK)
	{
		return rc;
	}

	return ResultCode::OK;
}

