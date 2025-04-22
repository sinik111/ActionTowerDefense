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

	StaticBackground* pBg = new StaticBackground(ResourceManager::Get().GetImage(L"Title", L"TitleBackground"));
	pBg->Initialize();
	m_Objects.push_back(pBg);

	ScreenTextUI* pTextUI = new ScreenTextUI(L"Game", Vector2(100.0f, 100.0f), Gdiplus::Color(0, 1, 0), 24);
	pTextUI->Initialize();
	m_Objects.push_back(pTextUI);
}

void TitleScene::Exit()
{
	Debug::Log("Exit Title Scene - TitleScene::Exit");

	__super::Exit();
}

void TitleScene::Unload()
{
	Debug::Log("Unload Title Scene - TitleScene::Unload");

	ResourceManager::Get().ReleaseResources(L"Title");
}

void TitleScene::Update()
{
	if (Input::IsKeyReleased('1'))
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

