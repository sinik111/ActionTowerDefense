#include "framework.h"
#include "IceTower.h"

#include "MyMath.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MyTime.h"
#include "ResourceManager.h"
#include "Enemy.h"
#include "CollisionManager.h"

IceTower::IceTower(const Vector2& position)
	: Tower::Tower(position), m_SlowRate(0.8f), m_SlowTime(1.0f),
	m_MaxAttackCount(3)
{
	m_Damage = 3.0f;
}

void IceTower::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"IceTower");

	m_Collider = Collider(m_Position, 400.0f);
}

void IceTower::Destroy()
{
	__super::Destroy();
}

void IceTower::Update()
{
	if (!m_TargetObjects.empty())
	{
		m_TargetObjects.clear();
	}

	if (!m_pInRangeObjects.empty())
	{
		std::sort(m_pInRangeObjects.begin(), m_pInRangeObjects.end(),
			[this](const Object* a, const Object* b) {
				return this->NearestComparer(a, b);
			});
	
		for (int i = 0; i < m_MaxAttackCount && i < m_pInRangeObjects.size(); ++i)
		{
			m_TargetObjects.push_back(std::move(m_pInRangeObjects[i]));
		}

		m_pInRangeObjects.clear();

		for (auto& object : m_TargetObjects)
		{
			object->Collide(this, L"Iceball");
		}
	}

	CollisionManager::Get().RegisterGameObject(L"TowerRange", this);

	__super::Update();
}

void IceTower::Render(const Camera& camera) const
{
	if (!m_TargetObjects.empty())
	{
		for (auto& object : m_TargetObjects)
		{
			Vector2 cameraViewPos = camera.ToCameraView(m_Position);
			Vector2 cameraViewTargetPos = camera.ToCameraView(object->GetPosition());

			GDIRenderer::Get().DrawLine(Gdiplus::Color(218, 255, 255), 7,
				Gdiplus::Point((int)cameraViewPos.x, (int)cameraViewPos.y),
				Gdiplus::Point((int)cameraViewTargetPos.x, (int)cameraViewTargetPos.y));
		}
	}

	__super::Render(camera);
}

void IceTower::Upgrade()
{
	__super::Upgrade();

	if (m_Level == 2)
	{
		m_Damage = 5.f;
		m_SlowRate = 0.65f;
		m_MaxAttackCount = 5;
	}
	else if (m_Level == 3)
	{
		m_Damage = 7.f;
		m_SlowRate = 0.5f;
		m_MaxAttackCount = 7;
	}
}


bool IceTower::NearestComparer(const Object* a, const Object* b)
{
	return Vector2::SquareDistance(m_Position, a->GetPosition()) < Vector2::SquareDistance(m_Position, b->GetPosition());
}

float IceTower::GetDamage()
{
	return m_Damage * MyTime::DeltaTime();
}

float IceTower::GetSlowTime()
{
	return m_SlowTime;
}

float IceTower::GetSlowRate()
{
	return m_SlowRate;
}
