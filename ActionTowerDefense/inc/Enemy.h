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
	int m_MoveIndex;
	float m_MoveSpeed;

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
	void Collide(Object& object, const std::wstring& groupName) override;
};