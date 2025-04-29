#include "framework.h"
#include "LightningTower.h"

#include "MyMath.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MyTime.h"
#include "ResourceManager.h"
#include "CollisionManager.h"

LightningTower::LightningTower(const Vector2& position)
	: Tower::Tower(position), m_pAttackImage(nullptr), m_AttackDuration(0.3f),
	m_IsStartAttack(false), m_IsCollide(false), m_AttackDurationTimer(0.0f)
{
	m_AttackRate = 1.0f;
	m_Damage = 100;
}

void LightningTower::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"LightningTower");
	m_pAttackImage = ResourceManager::Get().GetImage(L"Play", L"LightningNova");

	m_Collider = Collider(m_Position, 250.0f);
}

void LightningTower::Destroy()
{
	__super::Destroy();
}

void LightningTower::Update()
{
	if (!m_IsStartAttack)
	{
		m_AttackTimer += MyTime::DeltaTime();
		if (m_AttackRate < m_AttackTimer)
		{
			m_AttackTimer = 0.0f;
			m_IsStartAttack = true;
		}
	}

	if (m_IsStartAttack)
	{
		m_AttackDurationTimer += MyTime::DeltaTime();
		if (!m_IsCollide)
		{
			m_IsCollide = true;
			CollisionManager::Get().RegisterGameObject(L"TowerRange", this);
		}

		if (m_AttackDurationTimer > m_AttackDuration)
		{
			m_IsStartAttack = false;
			m_IsCollide = false;
			m_AttackDurationTimer = 0.0f;
		}
	}

	if (m_IsCollide && !m_pInRangeObjects.empty())
	{
		for (auto& object : m_pInRangeObjects)
		{
			object->Collide(this, L"LightningNova");
		}

		m_pInRangeObjects.clear();
	}

	__super::Update();
}

void LightningTower::Render(const Camera& camera) const
{
	if (m_IsStartAttack)
	{
		Gdiplus::Rect dstRect;
		dstRect.Width = m_pAttackImage->GetWidth();
		dstRect.Height = m_pAttackImage->GetHeight();

		Vector2 cameraViewPos = camera.ToCameraView(m_Position);

		dstRect.X = (int)(cameraViewPos.x - dstRect.Width / 2);
		dstRect.Y = (int)(cameraViewPos.y - dstRect.Height / 2);

		GDIRenderer::Get().DrawImage(m_pAttackImage, dstRect);
	}

	__super::Render(camera);
}

void LightningTower::Upgrade()
{
	__super::Upgrade();

	if (m_Level == 2)
	{
		m_Damage = 40;
		m_AttackRate = 0.8f;
	}
	else if (m_Level == 3)
	{
		m_Damage = 50;
		m_AttackRate = 0.5f;
	}
}

float LightningTower::GetDamage()
{
	return m_Damage;
}
