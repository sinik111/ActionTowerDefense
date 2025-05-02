#include "framework.h"
#include "Enemy.h"

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "Camera.h"
#include "GameData.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "MyMath.h"
#include "Fireball.h"
#include "CollisionManager.h"
#include "IceTower.h"
#include "LightningTower.h"
#include "Player.h"
#include "CenterCrystal.h"
#include "Input.h"
#include "Constant.h"

Enemy::Enemy(const Vector2& position, const std::vector<Vector2>& moveData, EnemyType type)
	: m_MoveData(moveData), m_MoveIndex(0), m_MoveSpeed(0.0f),
	m_Hp(0.0f), m_MaxHp(0.0f), m_IsSlowed(false), m_SlowRate(1.0f), m_SlowTimer(0.0f),
	m_HpPosition{}, m_Type(type), m_IsShocked(false), m_ShockTimer(0.0f),
	m_ShockMultiplier(1.0f)
{
	m_Position = position;
	m_RenderLayer = RenderLayer::Object;
}

void Enemy::Initialize()
{
	switch (m_Type)
	{
	case EnemyType::Circle:
		m_Color = Gdiplus::Color(0, 0, 255);
		m_MoveSpeed = 100.0f;
		m_Hp = 100.0f;
		m_MaxHp = 100.0f;
		break;

	case EnemyType::Rectangle:
		m_Color = Gdiplus::Color(255, 130, 0);
		m_MoveSpeed = 50.0f;
		m_Hp = 300.0f;
		m_MaxHp = 300.0f;
		break;

	case EnemyType::Triangle:
		m_Color = Gdiplus::Color(0, 255, 0);
		m_MoveSpeed = 200.0f;
		m_Hp = 50.0f;
		m_MaxHp = 50.0f;
		break;
	}
	
	m_Collider = Collider(m_Position, (float)(ENEMY_SIZE / 2));

	m_HpPosition[0] = Vector2(-(float)(ENEMY_SIZE / 2), -(float)(ENEMY_SIZE / 2 + 10.0f));
	m_HpPosition[1] = Vector2((float)(ENEMY_SIZE / 2), -(float)(ENEMY_SIZE / 2 + 10.0f));
}

void Enemy::Destroy()
{
	__super::Destroy();
}

void Enemy::Update()
{
	if (m_IsSlowed)
	{
		m_SlowTimer -= MyTime::DeltaTime();

		if (m_SlowTimer <= 0)
		{
			m_IsSlowed = false;
			m_SlowRate = 1.0f;
		}
	}

	if (m_IsShocked)
	{
		m_ShockTimer -= MyTime::DeltaTime();

		if (m_ShockTimer <= 0)
		{
			m_IsShocked = false;
			m_ShockMultiplier = 1.0f;
		}
	}

	if (m_MoveIndex < m_MoveData.size())
	{
		Vector2 destination = m_MoveData[m_MoveIndex];

		if (Vector2::SquareDistance(m_Position, destination) < Square(m_MoveSpeed * m_SlowRate * MyTime::DeltaTime()))
		{
			m_Position = destination;
			++m_MoveIndex;
		}
		else
		{
			Vector2 direction = Vector2::Direction(destination, m_Position);

			m_Position += direction * m_MoveSpeed * m_SlowRate * MyTime::DeltaTime();
		}
	}
	else
	{
		Destroy();
		GameData::Get().GetCenterCrystal()->TakeDamage();
	}

	if (!m_IsDestroyed)
	{
		m_Collider.UpdateCollider(m_Position);

		GameData::Get().RegisterMiniMapInfo(m_Position, MiniMapObjectType::Enemy);
		const Camera* camera = SceneManager::Get().GetCurrentCamera();
		if (!camera->IsOutOfView(m_Position, ENEMY_SIZE, ENEMY_SIZE))
		{
			Vector2 mousePos = camera->ToWorldView(Input::GetCursorPosition());
			if (CollisionManager::Get().IsCircleCollide(m_Collider, Collider(mousePos, 1.0f)))
			{
				GameData::Get().SetEnemyInfo(EnemyInfo((int)m_Type, m_Hp, m_MaxHp, m_MoveSpeed,
					m_SlowRate,m_ShockMultiplier, m_IsSlowed, m_IsShocked));
			}

			RenderManager::Get().AddObject(m_RenderLayer, this);
		}

		CollisionManager::Get().RegisterGameObject(L"Enemy", this);
	}
}

void Enemy::Render(const Camera& camera) const
{
	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	Gdiplus::Rect dstRect;
	dstRect.X = (int)(cameraViewPos.x - ENEMY_SIZE / 2);
	dstRect.Y = (int)(cameraViewPos.y - ENEMY_SIZE / 2);
	dstRect.Width = ENEMY_SIZE;
	dstRect.Height = ENEMY_SIZE;

	switch (m_Type)
	{
	case EnemyType::Circle:
		GDIRenderer::Get().DrawFillCircle(m_Color, dstRect);
		break;

	case EnemyType::Rectangle:
		GDIRenderer::Get().DrawFillRectangle(m_Color, dstRect);
		break;

	case EnemyType::Triangle:
		GDIRenderer::Get().DrawFillTriangle(m_Color, dstRect);
		break;
	}
	
	Vector2 lineStart = m_HpPosition[0] + cameraViewPos;
	Vector2 lineEnd = Vector2::Lerp(m_HpPosition[0], m_HpPosition[1], m_Hp / m_MaxHp) + cameraViewPos;

	GDIRenderer::Get().DrawLine(Gdiplus::Color(255, 0, 0), 4,
		Gdiplus::Point((int)lineStart.x, (int)lineStart.y),
		Gdiplus::Point((int)lineEnd.x, (int)lineEnd.y));
}

void Enemy::Collide(Object* object, const std::wstring& groupName)
{
	if (groupName == L"Fireball")
	{
		Fireball* pFireball = dynamic_cast<Fireball*>(object);

		m_Hp -= pFireball->GetDamage() * m_ShockMultiplier;

		m_Hp = Clamp(m_Hp, 0.0f, m_MaxHp);

		if (m_Hp == 0.0f)
		{
			GameData::Get().AddGold(10);

			Destroy();
		}
	}
	else if (groupName == L"Iceball")
	{
		IceTower* pIceTower = dynamic_cast<IceTower*>(object);

		m_Hp -= pIceTower->GetDamage() * m_ShockMultiplier;

		m_Hp = Clamp(m_Hp, 0.0f, m_MaxHp);

		m_SlowRate = pIceTower->GetSlowRate();
		m_SlowTimer = pIceTower->GetSlowTime();

		if (m_Hp == 0.0f)
		{
			GameData::Get().AddGold(10);

			Destroy();
		}

		m_IsSlowed = true;
	}
	else if (groupName == L"LightningNova")
	{
		LightningTower* pLightningTower = dynamic_cast<LightningTower*>(object);

		m_IsShocked = true;
		m_ShockTimer = 5.0f;

		m_Hp -= pLightningTower->GetDamage() * m_ShockMultiplier;

		m_ShockMultiplier = pLightningTower->GetMulitplier();

		m_Hp = Clamp(m_Hp, 0.0f, m_MaxHp);

		if (m_Hp == 0.0f)
		{
			GameData::Get().AddGold(10);

			Destroy();
		}
	}
	else if (groupName == L"PlayerAttack")
	{
		Player* pPlayer = dynamic_cast<Player*>(object);

		m_Hp -= pPlayer->GetDamage() * m_ShockMultiplier;

		m_Hp = Clamp(m_Hp, 0.0f, m_MaxHp);

		if (m_Hp == 0.0f)
		{
			GameData::Get().AddGold(10);

			Destroy();
		}
	}
}

bool Enemy::GetIsSlowed() const
{
	return m_IsSlowed;
}
