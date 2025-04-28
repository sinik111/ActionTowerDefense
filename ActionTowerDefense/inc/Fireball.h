#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class Fireball :
	public Object
{
protected:
	Gdiplus::Bitmap* m_pImage;
	Object* m_pTarget;
	Vector2 m_LastTargetPosition;
	float m_Damage;
	float m_Speed;

public:
	Fireball(const Vector2& position, float damage, Object* target);
	~Fireball() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void LateUpdate() override;
	void Render(const Camera& camera) const override;

public:
	float GetDamage();
};