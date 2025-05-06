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
#include "CenterCrystal.h"
#include "CrystalHp.h"
#include "GoldDisplay.h"
#include "EnemyInfoDisplay.h"
#include "TimerUI.h"
#include "Input.h"
#include "SceneManager.h"
#include "SoundManager.h"

PlayScene::PlayScene()
	: m_pTileMap(nullptr), m_pMiniMap(nullptr)
{
}

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

	SoundManager::Get().PlayBGM(L"PlayBGM", 0.1f);

	GameData::Get().Reset();

	Debug::Log("Enter Play Scene - PlayScene::Enter");



	GameData::Get().SetReadyStartTime();
	GameData::Get().SetStartGold();

	CreateObject<ScreenTextUI>(L"30초 후 적들이 몰려옵니다. 타워를 건설하세요.", Vector2(400.0f, 200.0f), Gdiplus::Color(255, 0, 0), 24, 5.0f);
	CreateObject<ScreenTextUI>(L"SPACE를 누르면 바로 시작합니다.", Vector2(450.0f, 230.0f), Gdiplus::Color(255, 0, 0), 24, 5.0f);
	CreateObject<Player>();
	m_pTileMap = CreateObject<TileMap>();
	m_pMiniMap = CreateObject<MiniMap>();
	CreateObject<CenterCrystal>();
	CreateObject<CrystalHp>();
	CreateObject<GoldDisplay>();
	CreateObject<EnemyInfoDisplay>();
	CreateObject<TimerUI>();
}

void PlayScene::Exit()
{
	SoundManager::Get().StopBGM();

	GameData::Get().Reset();

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
	CollisionManager::Get().CheckCollision(L"PlayerRange", L"Enemy");
	CollisionManager::Get().ClearCandidates();

	GameData::Get().ClearEnemyInfo();

	switch (GameData::Get().GetCurrentGameState())
	{
	case GameState::Ready:
		if (GameData::Get().GetRemainReadyTime() <= 0 ||
			Input::IsKeyReleased(VK_SPACE))
		{
			CreatePendingObject<ScreenTextUI>(
				L"5분동안 적들을 막고 크리스탈을 지키세요.", Vector2(420.0f, 150.0f), Gdiplus::Color::Red, 24, 5.0f);
			GameData::Get().SetCurrentGameState(GameState::Play);
			GameData::Get().SetPlayStartTime();
			GameData::Get().UpdatePlayTime();
			break;
		}

		GameData::Get().UpdateReadyTime();
		break;
	case GameState::Play:
		if (GameData::Get().GetRemainPlayTime() <= 0)
		{
			if (GameData::Get().GetEnemyCount() == 0)
			{
				CreatePendingObject<ScreenTextUI>(L"승리했습니다. 적들을 모두 막았습니다.", Vector2(410.0f, 200.0f), Gdiplus::Color::Red, 24, 10.0f);
				CreatePendingObject<ScreenTextUI>(L"10초 후 메인화면으로 돌아갑니다.", Vector2(440.0f, 230.0f), Gdiplus::Color::Red, 24, 10.0f);
				CreatePendingObject<ScreenTextUI>(L"SPACE를 누르면 바로 돌아갑니다.", Vector2(445.0f, 260.0f), Gdiplus::Color::Red, 24, 10.0f);

				GameData::Get().SetCurrentGameState(GameState::Victory);
				GameData::Get().SetEndStartTime();
				GameData::Get().UpdateEndTime();
			}
		}
		else
		{
			GameData::Get().UpdatePlayTime();
		}
		break;
	case GameState::Defeat:
	case GameState::Victory:
		if (GameData::Get().GetRemainEndTime() <= 0 || Input::IsKeyReleased(VK_SPACE))
		{
			SceneManager::Get().ChangeScene(L"TitleScene");

			return;
		}
		else
		{
			GameData::Get().UpdateEndTime();
		}
		break;
	}

	

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