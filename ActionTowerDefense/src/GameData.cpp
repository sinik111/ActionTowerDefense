#include "framework.h"
#include "GameData.h"

GameData::GameData()
	: m_pPlayer(nullptr), m_pCenterCrystal(nullptr), m_Gold(0), m_hasEnemyInfo(false),
	m_EnemyInfo{}
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

void GameData::SetPlayStartTime()
{
	m_PlayStartTime = Clock::now();
}

float GameData::GetElapsedSeconds()
{
	return MyTime::GetElapsedSeconds(m_PlayStartTime);
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

void GameData::Clear()
{
	ClearMiniMapInfo();
	m_pPlayer = nullptr;
	m_pCenterCrystal = nullptr;
	m_Gold = 0;
	m_hasEnemyInfo = false;
}
