#include "framework.h"
#include "GameData.h"

GameData::GameData()
	: m_pPlayer(nullptr)
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
