#pragma once

#include "Singleton.h"
#include "Vector2.h"
#include "MyTime.h"

enum class MiniMapObjectType
{
	Player,
	Enemy,
	Tower,
	EnterGate,
	ExitGate
};

struct MiniMapInfo
{
	Vector2 position;
	MiniMapObjectType type;

	MiniMapInfo(const Vector2& position, MiniMapObjectType type)
		: position(position), type(type)
	{

	}
};

class Player;

class GameData :
	public Singleton<GameData>
{
	friend class Singleton<GameData>;

private:
	GameData();
	~GameData() = default;

private:
	std::vector<MiniMapInfo> m_MiniMapInfo;

	TimePoint m_PlayStartTime;

	Player* m_pPlayer;

public:
	void RegisterMiniMapInfo(const Vector2& position, MiniMapObjectType type);
	const std::vector<MiniMapInfo>& GetMiniMapInfo();
	void ClearMiniMapInfo();

	void SetPlayStartTime();
	float GetElapsedSeconds();

	void SetPlayer(Player* pPlayer);
	Player* GetPlayer();
};