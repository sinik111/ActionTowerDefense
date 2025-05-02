#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class CenterCrystal :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImage[2];

	Vector2 m_ImagePositions[5];
	float m_Radians[5];
	int m_Hp;
	float m_RotateSpeed;
	float m_RotateRadius;
	bool m_CanDraw[5];

public:
	CenterCrystal();
	~CenterCrystal() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	void TakeDamage();
	int GetHp();
};