#include "framework.h"
#include "FireTower.h"

#include "MyMath.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MyTime.h"
#include "ResourceManager.h"
#include "Fireball.h"
#include "GameData.h"
#include "Player.h"
#include "CollisionManager.h"

FireTower::FireTower(const Vector2& position)
	: Tower::Tower(position)
{
	m_AttackRate = 1.0f;
	m_Damage = 20;
}

void FireTower::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"FireTower");

	m_Collider = Collider(m_Position, 400.0f);
}

void FireTower::Destroy()
{
	__super::Destroy();
}

void FireTower::Update()
{
	m_AttackTimer += MyTime::DeltaTime();
	if (m_AttackRate < m_AttackTimer)
	{
		if (!m_pInRangeObjects.empty())
		{
			std::sort(m_pInRangeObjects.begin(), m_pInRangeObjects.end(),
				[this](const Object* a, const Object* b) {
					return this->NearestComparer(a, b);
				});

			SceneManager::Get().GetCurrentScene()->CreatePendingObject<Fireball>(m_Position, m_Damage,
				m_pInRangeObjects[0]);

			m_AttackTimer = 0.0f;
		}

		CollisionManager::Get().RegisterGameObject(L"TowerRange", this);
	}

	if (!m_pInRangeObjects.empty())
	{
		m_pInRangeObjects.clear();
	}

	__super::Update();
}

void FireTower::Render(const Camera& camera) const
{
	__super::Render(camera);
}

void FireTower::Upgrade()
{
	__super::Upgrade();

	if (m_Level == 2)
	{
		m_Damage = 50;
		//m_AttackRate = 0.8f;
	}
	else if (m_Level == 3)
	{
		m_Damage = 70;
		//m_AttackRate = 0.5f;
	}
}

bool FireTower::NearestComparer(const Object* a, const Object* b)
{
	return Vector2::SquareDistance(m_Position, a->GetPosition()) < Vector2::SquareDistance(m_Position, b->GetPosition());
}
