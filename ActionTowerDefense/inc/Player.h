#pragma once

#include "Object.h"
#include "AnimationController.h"
#include "GDIRenderer.h"

class Camera;

enum class PlayerAnimState
{
	IdleDown,
	IdleUp,
	IdleRight,
	IdleLeft,
	WalkDown,
	WalkUp,
	WalkRight,
	WalkLeft
};

enum class AttackPositionType
{
	Up,
	Down,
	Left,
	Right,
	LeftUp,
	RightUp,
	LeftDown,
	RightDown,
	Max
};

class Player
	: public Object
{
private:
	AnimationController m_animation_controller;

	std::vector<Object*> m_pInRangeObjects;
	Gdiplus::Bitmap* m_pAttackImage[(int)AttackPositionType::Max];
	Collider m_AttackCollider;
	Vector2 m_AttackDirection[(int)AttackPositionType::Max];

	AttackPositionType m_CurrentDirection;

	PlayerAnimState m_current_state;
	PlayerAnimState m_next_state;

	float m_ColliderDistance;
	float m_ImageDistance;

	float m_AttackDuration;
	float m_AttackDurationTimer;
	bool m_IsStartAttack;
	bool m_IsCollide;

	float m_Speed;

public:
	Player();
	~Player() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	void Collide(Object* object, const std::wstring& groupName) override;
};