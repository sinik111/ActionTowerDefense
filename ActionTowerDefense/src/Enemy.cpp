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

Enemy::Enemy(const Vector2& position, const std::vector<Vector2>& moveData)
	: m_pImage(nullptr), m_MoveData(moveData), m_MoveIndex(0), m_MoveSpeed(0.0f),
	m_Hp(0.0f), m_MaxHp(0.0f), m_IsSlowed(false), m_SlowRate(1.0f), m_SlowTimer(0.0f)
{
	m_Position = position;
}

void Enemy::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"Enemy");

	m_MoveSpeed = 100.0f;
	m_Hp = 100.0f;
	m_MaxHp = 100.0f;

	m_Collider = Collider(m_Position, (float)(m_pImage->GetWidth() / 2));
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

	if (!m_IsDestroyed)
	{
		m_Collider.UpdateCollider(m_Position);

		GameData::Get().RegisterMiniMapInfo(m_Position, MiniMapObjectType::Enemy);
		if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position, m_pImage->GetWidth(), m_pImage->GetHeight()))
		{
			RenderManager::Get().AddObject(m_RenderLayer, this);
		}

		CollisionManager::Get().RegisterGameObject(L"Enemy", this);
	}
}

void Enemy::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect, srcRect;
	srcRect.X = 0;
	srcRect.Y = 0;
	dstRect.Width = srcRect.Width = m_pImage->GetWidth();
	dstRect.Height = srcRect.Height = m_pImage->GetHeight();

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	dstRect.X = (int)(cameraViewPos.x - srcRect.Width / 2);
	dstRect.Y = (int)(cameraViewPos.y - srcRect.Height / 2);

	GDIRenderer::Get().DrawImage(m_pImage, dstRect, srcRect);
}

void Enemy::Collide(Object* object, const std::wstring& groupName)
{
	if (groupName == L"Fireball")
	{
		Fireball* pFireball = dynamic_cast<Fireball*>(object);

		m_Hp -= pFireball->GetDamage();

		m_Hp = Clamp(m_Hp, 0.0f, m_MaxHp);

		if (m_Hp == 0.0f)
		{
			Destroy();
		}
	}

	if (groupName == L"Iceball")
	{
		IceTower* pIceTower = dynamic_cast<IceTower*>(object);

		m_Hp -= pIceTower->GetDamage();

		m_Hp = Clamp(m_Hp, 0.0f, m_MaxHp);

		m_SlowRate = pIceTower->GetSlowRate();
		m_SlowTimer = pIceTower->GetSlowTime();

		if (m_Hp == 0.0f)
		{
			Destroy();
		}

		m_IsSlowed = true;
	}
}

bool Enemy::GetIsSlowed() const
{
	return m_IsSlowed;
}
