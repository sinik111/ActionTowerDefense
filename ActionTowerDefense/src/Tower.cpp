#include "framework.h"
#include "Tower.h"

#include "MyMath.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MyTime.h"
#include "CollisionManager.h"
#include "GameData.h"

Tower::Tower(const Vector2& position)
	: m_pImage(nullptr), m_Damage(0), m_AttackRate(0.0f), m_AttackTimer(0.0f),
	m_Level(1), m_Radians{}
{
	m_Position = position - Vector2::Up;
	m_RenderLayer = RenderLayer::Object;
	m_RotateRadius = 20.0f;
	m_RotateSpeed = PI * 2.0f / 2.0f;
}

void Tower::Initialize()
{

}

void Tower::Destroy()
{
	__super::Destroy();
}

void Tower::Update()
{
	if (!m_IsDestroyed)
	{
		if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position, m_pImage->GetWidth(), m_pImage->GetHeight()))
		{
			for (int i = 0; i < m_Level; ++i)
			{
				float x = m_Position.x + m_RotateRadius * cosf(m_Radians[i]);
				float y = m_Position.y + m_RotateRadius * sinf(m_Radians[i]);

				m_ImagePosition[i] = Vector2(x, y);

				m_Radians[i] += m_RotateSpeed * MyTime::DeltaTime();
			}

			RenderManager::Get().AddObject(m_RenderLayer, this);

			GameData::Get().RegisterMiniMapInfo(m_Position, MiniMapObjectType::Tower);
		}
	}
}

void Tower::Render(const Camera& camera) const
{
	for (int i = 0; i < m_Level; ++i)
	{
		Gdiplus::Rect dstRect;
		dstRect.Width = m_pImage->GetWidth();
		dstRect.Height = m_pImage->GetHeight();

		Vector2 cameraViewPos = camera.ToCameraView(m_ImagePosition[i]);

		dstRect.X = (int)(cameraViewPos.x - dstRect.Width / 2);
		dstRect.Y = (int)(cameraViewPos.y - dstRect.Height / 2);

		GDIRenderer::Get().DrawImage(m_pImage, dstRect);
	}
}

void Tower::Upgrade()
{
	++m_Level;

	if (m_Level == 2)
	{
		m_Radians[0] = 0.0f;
		m_Radians[1] = PI;
	}
	else
	{
		m_Radians[0] = 0.0f;
		m_Radians[1] = PI * 2.0f * (1.0f / 3.0f);
		m_Radians[2] = PI * 2.0f * (2.0f / 3.0f);
	}
}

void Tower::Collide(Object* object, const std::wstring& groupName)
{
	if (groupName == L"Enemy")
	{
		m_pInRangeObjects.push_back(object);
	}
}