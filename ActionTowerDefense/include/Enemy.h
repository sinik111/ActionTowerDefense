#pragma once

#include "Object.h"
#include "GDIRenderer.h"

enum class EnemyType
{
	Circle,
	Rectangle,
	Triangle,
	Max
};

class Camera;

class Enemy :
	public Object
{
private:
	const std::vector<Vector2>& m_MoveData;
	Vector2 m_HpPosition[2];
	int m_MoveIndex;
	float m_Hp;
	float m_MaxHp;
	float m_MoveSpeed;
	float m_SlowRate;
	float m_SlowTimer;
	float m_ShockTimer;
	float m_ShockMultiplier;
	Gdiplus::Color m_Color;
	EnemyType m_Type;
	bool m_IsSlowed;
	bool m_IsShocked;

public:
	Enemy(const Vector2& position, const std::vector<Vector2>& moveData, EnemyType type);
	~Enemy() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	void Collide(Object* object, const std::wstring& groupName) override;
	bool GetIsSlowed() const;
};