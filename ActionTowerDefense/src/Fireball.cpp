#include "framework.h"
#include "Fireball.h"

#include "ResourceManager.h"
#include "Camera.h"
#include "MyTime.h"
#include "MyMath.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"

Fireball::Fireball(const Vector2& position, float damage, Object* target)
	: m_pImage(nullptr), m_pTarget(target), m_Damage(damage), m_Speed(300.0f)
{
	m_Position = position;
	m_RenderLayer = RenderLayer::Object;
}

void Fireball::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"Fireball");

	m_Collider = Collider(m_Position, (float)m_pImage->GetWidth() / 2);
}

void Fireball::Destroy()
{
	__super::Destroy();
}

void Fireball::Update()
{
	if (!m_IsDestroyed)
	{
		if (m_pTarget != nullptr && m_pTarget->IsDestroyed())
		{
			m_pTarget = nullptr;
		}

		if (m_pTarget != nullptr)
		{
			m_LastTargetPosition = m_pTarget->GetPosition();

			Vector2 direction = Vector2::Direction(m_LastTargetPosition, m_Position);

			m_Position += direction * m_Speed * MyTime::DeltaTime();

			m_Collider.UpdateCollider(m_Position);

			if (CollisionManager::Get().IsCircleCollide(m_pTarget->GetCollider(), m_Collider))
			{
				m_pTarget->Collide(this, L"Fireball");
				Destroy();
			}
		}
		else
		{
			if (Vector2::SquareDistance(m_LastTargetPosition, m_Position) < Square(m_Speed * MyTime::DeltaTime()))
			{
				Destroy();
			}
			else
			{
				Vector2 direction = Vector2::Direction(m_LastTargetPosition, m_Position);

				m_Position += direction * m_Speed * MyTime::DeltaTime();
			}
		}

		if (!m_IsDestroyed)
		{
			if (m_pTarget != nullptr)
			{
				SceneManager::Get().GetCurrentScene()->AddLateUpdateObject(this);
			}

			if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position, m_pImage->GetWidth(), m_pImage->GetHeight()))
			{
				RenderManager::Get().AddObject(m_RenderLayer, this);
			}
		}
	}
}

void Fireball::LateUpdate()
{
	if (m_pTarget != nullptr && m_pTarget->IsDestroyed())
	{
		m_pTarget = nullptr;
	}
}

void Fireball::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect;
	dstRect.Width = m_pImage->GetWidth();
	dstRect.Height = m_pImage->GetHeight();

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	dstRect.X = (int)(cameraViewPos.x - dstRect.Width / 2);
	dstRect.Y = (int)(cameraViewPos.y - dstRect.Height / 2);

	GDIRenderer::Get().DrawImage(m_pImage, dstRect);
}

float Fireball::GetDamage()
{
	return m_Damage;
}
