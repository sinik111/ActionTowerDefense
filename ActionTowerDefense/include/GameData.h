#pragma once

#include "Singleton.h"
#include "Vector2.h"
#include "MyTime.h"

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
	GameData();
	~GameData() = default;

private:
	std::vector<MiniMapInfo> m_MiniMapInfo;

	TimePoint m_PlayStartTime;

	Player* m_pPlayer;
	CenterCrystal* m_pCenterCrystal;

	EnemyInfo m_EnemyInfo;

	int m_Gold;

	bool m_hasEnemyInfo;

public:
	void RegisterMiniMapInfo(const Vector2& position, MiniMapObjectType type);
	const std::vector<MiniMapInfo>& GetMiniMapInfo();
	void ClearMiniMapInfo();

	void SetPlayStartTime();
	float GetElapsedSeconds();

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

public:
	void Clear();
};