#include "framework.h"
#include "CenterCrystal.h"

#include "Constant.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "GameData.h"
#include "SceneManager.h"
#include "MyMath.h"

CenterCrystal::CenterCrystal()
	: m_Hp(5), m_pImage{}, m_RotateRadius(40.0f), m_RotateSpeed(PI * 2.0f / 6.0f),
	m_CanDraw{}
{
	GameData::Get().SetCenterCrystal(this);

	m_RenderLayer = RenderLayer::Object;
	m_Position = Vector2(TILE_SIZE * MAP_SIZE / 2, TILE_SIZE * MAP_SIZE / 2);
	m_Radians[0] = 0.0f;
	m_Radians[1] = PI * 2.0f * (1.0f / 5.0f);
	m_Radians[2] = PI * 2.0f * (2.0f / 5.0f);
	m_Radians[3] = PI * 2.0f * (3.0f / 5.0f);
	m_Radians[4] = PI * 2.0f * (4.0f / 5.0f);
}

void CenterCrystal::Initialize()
{
	m_pImage[0] = ResourceManager::Get().GetImage(L"Play", L"CenterCrystal");
	m_pImage[1] = ResourceManager::Get().GetImage(L"Play", L"CenterCrystalRotate");
}

void CenterCrystal::Destroy()
{
	__super::Destroy();
}

void CenterCrystal::Update()
{
	if (!m_IsDestroyed)
	{
		if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position,
			m_pImage[0]->GetWidth(), m_pImage[0]->GetHeight()))
		{
			for (int i = 0; i < m_Hp; ++i)
			{
				if (m_Radians[i] > PI * 2.0f * (2.8f / 4.0f) &&
					m_Radians[i] < PI * 2.0f * (3.15f / 4.0f))
				{
					m_CanDraw[i] = false;
				}
				else
				{
					m_CanDraw[i] = true;
				}

				float x = m_Position.x + (m_RotateRadius * 2.0f) * cosf(m_Radians[i]);
				float y = m_Position.y + m_RotateRadius * sinf(m_Radians[i]);

				m_ImagePositions[i] = Vector2(x, y);

				m_Radians[i] += m_RotateSpeed * MyTime::DeltaTime();

				if (m_Radians[i] > PI * 2.0f)
				{
					m_Radians[i] -= PI * 2.0f;
				}
			}

			RenderManager::Get().AddObject(m_RenderLayer, this);
		}
	}
}

void CenterCrystal::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect;
	dstRect.Width = m_pImage[0]->GetWidth();
	dstRect.Height = m_pImage[0]->GetHeight();

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	dstRect.X = (int)(cameraViewPos.x - dstRect.Width / 2);
	dstRect.Y = (int)(cameraViewPos.y - dstRect.Height / 2);

	GDIRenderer::Get().DrawImage(m_pImage[0], dstRect);

	for (int i = 0; i < m_Hp; ++i)
	{
		if (m_CanDraw[i])
		{
			Gdiplus::Rect dstRect;
			dstRect.Width = m_pImage[1]->GetWidth();
			dstRect.Height = m_pImage[1]->GetHeight();

			Vector2 cameraViewPos = camera.ToCameraView(m_ImagePositions[i]);

			dstRect.X = (int)(cameraViewPos.x - dstRect.Width / 2);
			dstRect.Y = (int)(cameraViewPos.y - dstRect.Height / 2);

			GDIRenderer::Get().DrawImage(m_pImage[1], dstRect);
		}
	}
}

void CenterCrystal::TakeDamage()
{
	if (m_Hp != 0)
	{
		--m_Hp;

		switch (m_Hp)
		{
		case 4:
			m_Radians[0] = 0.0f;
			m_Radians[1] = PI * 2.0f * (1.0f / 4.0f);
			m_Radians[2] = PI * 2.0f * (2.0f / 4.0f);
			m_Radians[3] = PI * 2.0f * (3.0f / 4.0f);
			break;
		case 3:
			m_Radians[0] = 0.0f;
			m_Radians[1] = PI * 2.0f * (1.0f / 3.0f);
			m_Radians[2] = PI * 2.0f * (2.0f / 3.0f);
			break;
		case 2:
			m_Radians[0] = 0.0f;
			m_Radians[1] = PI * 2.0f * (1.0f / 2.0f);
			break;
		case 1:
			m_Radians[0] = 0.0f;
			break;

		case 0:
			// defeat
			break;
		}
	}
}

int CenterCrystal::GetHp()
{
	return m_Hp;
}
