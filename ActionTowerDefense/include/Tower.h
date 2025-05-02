#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class Camera;

class Tower :
	public Object
{
protected:
	Gdiplus::Bitmap* m_pImage;
	std::vector<Object*> m_pInRangeObjects;
	Vector2 m_ImagePosition[3];
	float m_Damage;
	float m_AttackRate;
	float m_AttackTimer;
	float m_RotateRadius;
	float m_RotateSpeed;
	float m_Radians[3];
	int m_Level;

public:
	Tower(const Vector2& position);
	~Tower() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	virtual void Upgrade();
	void Collide(Object* object, const std::wstring& groupName) override;
};