#include "framework.h"
#include "GameData.h"

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
