#include "framework.h"
#include "LightningTower.h"

#include "MyMath.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MyTime.h"
#include "ResourceManager.h"

LightningTower::LightningTower(const Vector2& position)
	: Tower::Tower(position)
{
	m_AttackRate = 1.0f;
	m_Damage = 10;
}

void LightningTower::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"LightningTower");
}

void LightningTower::Destroy()
{
	__super::Destroy();
}

void LightningTower::Update()
{
	__super::Update();
}

void LightningTower::Render(const Camera& camera) const
{
	__super::Render(camera);
}