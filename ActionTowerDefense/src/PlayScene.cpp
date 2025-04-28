#include "framework.h"
#include "PlayScene.h"

#include <sstream>

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
#include "EnterGate.h"
#include "GameData.h"
#include "TowerPlace.h"
#include "CollisionManager.h"

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

	GameData::Get().SetPlayStartTime();

	//CreateObject<StaticBackground>(ResourceManager::Get().GetImage(L"Play", L"PlayBackground"));
	CreateObject<ScreenTextUI>(L"Game", Vector2(200.0f, 200.0f), Gdiplus::Color(0, 0, 1), 36);
	CreateObject<Player>();
	CreateObject<TileMap>();
	CreateObject<MiniMap>();
	CreateEnterGates();
	CreateTowerPlaces();
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
	CollisionManager::Get().CheckCollision(L"TowerRange", L"Enemy");
	CollisionManager::Get().ClearCandidates();

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

void PlayScene::CreateEnterGates()
{
	const std::wstring& gateData = ResourceManager::Get().GetString(L"Play", L"EnterGateCreateData");

	std::wstringstream wss(gateData);

	int row, column, gateNumber;

	while (wss >> row >> column >> gateNumber)
	{
		CreateObject<EnterGate>(gateNumber, row, column);
	}
}

void PlayScene::CreateTowerPlaces()
{
	const std::wstring& gateData = ResourceManager::Get().GetString(L"Play", L"TowerPlaceData");

	std::wstringstream wss(gateData);

	int row, column;

	while (wss >> row >> column)
	{
		CreateObject<TowerPlace>(row, column);
	}
}