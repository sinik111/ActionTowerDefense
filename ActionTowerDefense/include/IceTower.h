#pragma once

#include "Tower.h"
#include "GDIRenderer.h"

class Camera;

class IceTower :
	public Tower
{
private:
	std::vector<Object*> m_TargetObjects;
	float m_SlowTime;
	float m_SlowRate;
	int m_MaxAttackCount;

public:
	IceTower(const Vector2& position);
	~IceTower() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	virtual void Upgrade() override;

private:
	bool NearestComparer(const Object* a, const Object* b);

public:
	float GetDamage();
	float GetSlowTime();
	float GetSlowRate();
};