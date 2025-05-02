#pragma once

#include "Tower.h"
#include "GDIRenderer.h"

class Camera;

class LightningTower :
	public Tower
{
private:
	Gdiplus::Bitmap* m_pAttackImage;
	float m_AttackDuration;
	float m_AttackDurationTimer;
	bool m_IsStartAttack;
	bool m_IsCollide;

public:
	LightningTower(const Vector2& position);
	~LightningTower() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	void Upgrade() override;
	float GetDamage();
};