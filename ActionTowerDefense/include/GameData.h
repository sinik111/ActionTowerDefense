#pragma once

#include "Singleton.h"
#include "Vector2.h"
#include "MyTime.h"

enum class GameState
{
	Ready,
	Play,
	Victory,
	Defeat
};

enum class MiniMapObjectType
{
	Player,
	Enemy,
	Tower,
	Max
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

struct EnemyInfo
{
	int type;
	float hp;
	float maxHp;
	float speed;
	float slowRate;
	float multiplier;
	bool isSlowed;
	bool isShocked;

	EnemyInfo()
		: type(0), hp(0.0f), maxHp(0.0f), speed(0.0f), slowRate(0.0f),
		multiplier(0.0f), isSlowed(false), isShocked(false)
	{

	}

	EnemyInfo(int type, float hp, float maxHp, float speed,
		float slowRate, float multiplier, bool isSlowed, bool isShocked)
		: type(type), hp(hp), maxHp(maxHp), speed(speed),
		slowRate(slowRate), multiplier(multiplier), isSlowed(isSlowed),
		isShocked(isShocked)
	{

	}


	bool operator==(const EnemyInfo& other)
	{
		if (this->type != other.type)
			return false;
		if ((int)this->hp != (int)other.hp)
			return false;
		if (this->isSlowed != other.isSlowed)
			return false;
		if (this->isShocked != other.isShocked)
			return false;

		return true;
	}
};

class Player;
class CenterCrystal;

class GameData :
	public Singleton<GameData>
{
	friend class Singleton<GameData>;

private:
	std::vector<MiniMapInfo> m_MiniMapInfo;

	TimePoint m_ReadyTimePoint;
	TimePoint m_PlayTimePoint;
	TimePoint m_EndTimePoint;

	Player* m_pPlayer;
	CenterCrystal* m_pCenterCrystal;

	GameState m_GameState;

	EnemyInfo m_EnemyInfo;

	int m_Gold;
	float m_CurrentSpawnRate;
	int m_CurrentSpawnBurst;

	const int m_TotalReadyTime;
	int m_RemainReadyTime;

	const int m_TotalPlayTime;
	int m_RemainPlayTime;

	const int m_TotalEndTime;
	int m_RemainEndTime;

	int m_EnemyCount;

	bool m_hasEnemyInfo;

private:
	GameData();
	~GameData() = default;

public:
	void RegisterMiniMapInfo(const Vector2& position, MiniMapObjectType type);
	const std::vector<MiniMapInfo>& GetMiniMapInfo();
	void ClearMiniMapInfo();

	void SetReadyStartTime();
	int GetRemainReadyTime();

	void SetPlayStartTime();
	float GetElapsedSeconds();
	int GetRemainPlayTime();

	int GetRemainEndTime();
	void SetEndStartTime();

	void UpdatePlayTime();
	void UpdateReadyTime();
	void UpdateEndTime();

	void SetPlayer(Player* pPlayer);
	Player* GetPlayer();

	void SetCenterCrystal(CenterCrystal* pCenterCrystal);
	CenterCrystal* GetCenterCrystal();

	void SetStartGold();
	void AddGold(int gold);
	bool UseGold(int gold);
	int GetCurrentGold();

	void SetEnemyInfo(const EnemyInfo& enemyInfo);
	void ClearEnemyInfo();
	const EnemyInfo& GetEnemyInfo();
	bool HasEnemyInfo();

	float GetCurrentSpawnRate();
	int GetCurrentSpawnBurst();

	GameState GetCurrentGameState();
	void SetCurrentGameState(GameState state);

	void IncreaseEnemyCount();
	void DecreaseEnemyCount();
	int GetEnemyCount();

public:
	void Reset();
};