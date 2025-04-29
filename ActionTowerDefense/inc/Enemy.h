#pragma once

#include "Object.h"

namespace Gdiplus
{
	class Bitmap;
}

class Camera;

class Enemy :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImage;
	const std::vector<Vector2>& m_MoveData;
	Vector2 m_HpPosition[2];
	int m_MoveIndex;
	float m_Hp;
	float m_MaxHp;
	float m_MoveSpeed;
	float m_SlowRate;
	float m_SlowTimer;
	bool m_IsSlowed;

public:
	Enemy(const Vector2& position, const std::vector<Vector2>& moveData);
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