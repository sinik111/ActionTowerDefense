#include "framework.h"
#include "PlayScene.h"

#include "ResultCode.h"
#include "Debug.h"
#include "ResourceManager.h"
#include "FileLoader.h"
#include "GDIRenderer.h"
#include "StaticBackground.h"
#include "ScreenTextUI.h"
#include "Player.h"
#include "TileMap.h"
#include "MiniMap.h"

PlayScene::~PlayScene()
{
	Unload();
}

ResultCode PlayScene::Load()
{
	Debug::Log("Load Play Scene - PlayScene::Load");

	ResultCode rc = ResultCode::OK;

	rc = LoadResources();
	if (rc != ResultCode::OK)
	{
		return rc;
	}

	return ResultCode::OK;
}

void PlayScene::Enter()
{
	__super::Enter();

	Debug::Log("Enter Play Scene - PlayScene::Enter");

	CreateObject<StaticBackground>(ResourceManager::Get().GetImage(L"Play", L"PlayBackground"));
	CreateObject<ScreenTextUI>(L"Game", Vector2(200.0f, 200.0f), Gdiplus::Color(0, 0, 1), 36);
	CreateObject<Player>();
	CreateObject<TileMap>();
	CreateObject<MiniMap>();

	/*StaticBackground* pBg = new StaticBackground();
	pBg->Initialize();
	m_Objects.push_back(pBg);*/

	/*ScreenTextUI* pTextUI = new ScreenTextUI(L"Game", Vector2(200.0f, 200.0f), Gdiplus::Color(0, 0, 1), 36);
	pTextUI->Initialize();
	m_Objects.push_back(pTextUI);*/

	/*Player* pPlayer = new Player();
	pPlayer->Initialize();
	m_Objects.push_back(pPlayer);

	TileMap* pTileMap = new TileMap();
	pTileMap->Initialize();
	m_Objects.push_back(pTileMap);

	MiniMap* pMiniMap = new MiniMap();
	pMiniMap->Initialize();
	m_Objects.push_back(pMiniMap);*/
}

void PlayScene::Exit()
{
	Debug::Log("Exit Play Scene - PlayScene::Exit");

	__super::Exit();
}

void PlayScene::Unload()
{
	Debug::Log("Unload Play Scene - PlayScene::Unload");

	ResourceManager::Get().ReleaseResources(L"Play");
}

void PlayScene::Update()
{
	__super::Update();
}

ResultCode PlayScene::LoadResources()
{
	ResultCode rc = ResultCode::OK;

	std::wstring data = FileLoader::Get().LoadTextFile(L"data/PlaySceneResourceList.txt");
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

