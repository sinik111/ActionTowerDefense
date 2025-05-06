#include "framework.h"
#include "GameData.h"

GameData::GameData()
	: m_pPlayer(nullptr), m_pCenterCrystal(nullptr), m_Gold(0), m_hasEnemyInfo(false),
	m_EnemyInfo{}, m_CurrentSpawnRate(5.0f), m_CurrentSpawnBurst(10),
	m_GameState(GameState::Ready), m_RemainPlayTime(300), m_RemainReadyTime(30),
	m_EnemyCount(0), m_RemainEndTime(10), m_TotalPlayTime(300), m_TotalEndTime(10),
	m_TotalReadyTime(30)
{
}

void GameData::RegisterMiniMapInfo(const Vector2& position, MiniMapObjectType type)
{
	m_MiniMapInfo.emplace_back(position, type);
}

const std::vector<MiniMapInfo>& GameData::GetMiniMapInfo()
{
	return m_MiniMapInfo;
}

void GameData::ClearMiniMapInfo()
{
	m_MiniMapInfo.clear();
}

void GameData::SetReadyStartTime()
{
	m_ReadyTimePoint = Clock::now();
}

int GameData::GetRemainReadyTime()
{
	return m_RemainReadyTime;
}

void GameData::SetPlayStartTime()
{
	m_PlayTimePoint = Clock::now();
}

float GameData::GetElapsedSeconds()
{
	return MyTime::GetElapsedSeconds(m_PlayTimePoint);
}

int GameData::GetRemainPlayTime()
{
	return m_RemainPlayTime;
}

int GameData::GetRemainEndTime()
{
	return m_RemainEndTime;
}

void GameData::SetEndStartTime()
{
	m_EndTimePoint = Clock::now();
}

void GameData::UpdatePlayTime()
{
	m_RemainPlayTime = m_TotalPlayTime - (int)MyTime::GetElapsedSeconds(m_PlayTimePoint);
}

void GameData::UpdateReadyTime()
{
	m_RemainReadyTime = m_TotalReadyTime - (int)MyTime::GetElapsedSeconds(m_ReadyTimePoint);
}

void GameData::UpdateEndTime()
{
	m_RemainEndTime = m_TotalEndTime - (int)MyTime::GetElapsedSeconds(m_EndTimePoint);
}

void GameData::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

Player* GameData::GetPlayer()
{
	return m_pPlayer;
}

void GameData::SetCenterCrystal(CenterCrystal* pCenterCrystal)
{
	m_pCenterCrystal = pCenterCrystal;
}

CenterCrystal* GameData::GetCenterCrystal()
{
	return m_pCenterCrystal;
}

void GameData::SetStartGold()
{
	m_Gold = 1000;
}

void GameData::AddGold(int gold)
{
	m_Gold += gold;
}

bool GameData::UseGold(int gold)
{
	if (m_Gold < gold)
	{
		return false;
	}

	m_Gold -= gold;

	return true;
}

int GameData::GetCurrentGold()
{
	return m_Gold;
}

void GameData::SetEnemyInfo(const EnemyInfo& enemyInfo)
{
	m_EnemyInfo = enemyInfo;
	m_hasEnemyInfo = true;
}

void GameData::ClearEnemyInfo()
{
	m_hasEnemyInfo = false;
}

const EnemyInfo& GameData::GetEnemyInfo()
{
	return m_EnemyInfo;
}

bool GameData::HasEnemyInfo()
{
	return m_hasEnemyInfo;
}

float GameData::GetCurrentSpawnRate()
{
	return m_CurrentSpawnRate;
}

int GameData::GetCurrentSpawnBurst()
{
	return m_CurrentSpawnBurst;
}

GameState GameData::GetCurrentGameState()
{
	return m_GameState;
}

void GameData::SetCurrentGameState(GameState state)
{
	m_GameState = state;
}

void GameData::IncreaseEnemyCount()
{
	++m_EnemyCount;
}

void GameData::DecreaseEnemyCount()
{
	--m_EnemyCount;
}

int GameData::GetEnemyCount()
{
	return m_EnemyCount;
}

void GameData::Reset()
{
	ClearMiniMapInfo();
	m_pPlayer = nullptr;
	m_pCenterCrystal = nullptr;
	m_Gold = 0;
	m_hasEnemyInfo = false;
	m_CurrentSpawnRate = 5.0f;
	m_CurrentSpawnBurst = 10;
	m_GameState = GameState::Ready;
	m_RemainPlayTime = 300;
	m_RemainReadyTime = 30;
	m_RemainEndTime = 30;
	m_EnemyCount = 0;
}
