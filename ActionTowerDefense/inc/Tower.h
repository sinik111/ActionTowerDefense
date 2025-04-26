#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class Camera;

class Tower :
	public Object
{
protected:
	Gdiplus::Bitmap* m_pImage;
	int m_Damage;
	int m_AttackRate;
	int m_AttackTimer;
	int m_Level;

public:
	Tower() = default;
	~Tower() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};