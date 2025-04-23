#include "framework.h"
#include "Enemy.h"

Enemy::Enemy(const Vector2& position, const std::vector<Vector2>& moveData)
	: m_pImage(nullptr), m_MoveData(moveData)
{
}

void Enemy::Initialize()
{
}

void Enemy::Destroy()
{
}

void Enemy::Update()
{
}

void Enemy::Render(const Camera& camera) const
{
}

void Enemy::Collide(Object& object, const std::wstring& groupName)
{
}
