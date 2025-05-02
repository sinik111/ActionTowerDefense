#include "framework.h"
#include "TowerBuildButton.h"

#include "TowerPlace.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "Constant.h"
#include "CollisionManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "Collider.h"

TowerBuildButton::TowerBuildButton(TowerPlace* pTargetTowerPlace)
	: m_pTargetTowerPlace(pTargetTowerPlace), m_pTowerButtonImages{},
	m_IsButtonDown{}, m_pBuffer(nullptr), m_pGraphics(nullptr)
{
	m_RenderLayer = RenderLayer::ScreenUI;

	
}

TowerBuildButton::~TowerBuildButton()
{
	if (m_pGraphics != nullptr)
	{
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}

	if (m_pBuffer != nullptr)
	{
		delete m_pBuffer;
		m_pBuffer = nullptr;
	}
}

void TowerBuildButton::Initialize()
{
	m_PanelRect = Gdiplus::Rect(248, 584, 784, 136);

	Collider collider(Vector2((float)(m_PanelRect.X + 4), (float)(m_PanelRect.Y + 4)), 256, 128);

	m_ButtonCollider[0] = collider;

	collider.position.x += 260;
	m_ButtonCollider[1] = collider;

	collider.position.x += 260;
	m_ButtonCollider[2] = collider;

	m_ButtonBufferRects[0] = Gdiplus::Rect(4, 4, 256, 128);
	m_ButtonBufferRects[1] = Gdiplus::Rect(m_ButtonBufferRects[0].X + 260, 4, 256, 128);
	m_ButtonBufferRects[2] = Gdiplus::Rect(m_ButtonBufferRects[1].X + 260, 4, 256, 128);

	LoadTowerButtonImages();

	m_pBuffer = new Gdiplus::Bitmap(m_PanelRect.Width, m_PanelRect.Height, PixelFormat24bppRGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	DrawOnBuffer();
}

void TowerBuildButton::Destroy()
{
	__super::Destroy();
}

void TowerBuildButton::Update()
{
	for (int i = 0; i < (int)TowerType::MAX; ++i)
	{
		if (CollisionManager::Get().CheckUIRectContainPosition(m_ButtonCollider[i], Input::GetCursorPosition()))
		{
			if (Input::IsKeyPressed(VK_LBUTTON))
			{
				m_IsButtonDown[i] = true;
			}

			if (Input::IsKeyReleased(VK_LBUTTON) && m_IsButtonDown[i])
			{
				m_pTargetTowerPlace->UpgradeTower(TowerType(i));

				Destroy();

				m_IsButtonDown[i] = false;
			}
		}
		else
		{
			m_IsButtonDown[i] = false;
		}
	}

	if (Input::IsKeyReleased(VK_LBUTTON))
	{
		Destroy();
	}

	if (!m_IsDestroyed)
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

void TowerBuildButton::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawImage(m_pBuffer, m_PanelRect);
}

void TowerBuildButton::LoadTowerButtonImages()
{
	TowerState towerState = m_pTargetTowerPlace->GetTowerState();

	for (int i = 0; i < (int)TowerType::MAX; ++i)
	{
		if (i == (int)towerState.type)
		{
			if (towerState.level == 3)
			{
				m_pTowerButtonImages[i] = ResourceManager::Get().GetImage(L"Play", L"TowerLevelMax");
			}
			else
			{
				m_pTowerButtonImages[i] = ResourceManager::Get().GetImage(L"Play",
					TowerTypeName(i) + L"TowerButtonIcon" + std::to_wstring(towerState.level + 1));
			}
		}
		else
		{
			m_pTowerButtonImages[i] = ResourceManager::Get().GetImage(L"Play", TowerTypeName(i) + L"TowerButtonIcon1");
		}
	}
}

std::wstring TowerBuildButton::TowerTypeName(int type)
{
	switch (type)
	{
	case (int)TowerType::Fire:
		return std::wstring(L"Fire");

	case (int)TowerType::Ice:
		return std::wstring(L"Ice");

	case (int)TowerType::Lightning:
		return std::wstring(L"Lightning");
	}

	return L"";
}

void TowerBuildButton::DrawOnBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);

	for (int i = 0; i < (int)TowerType::MAX; ++i)
	{
		m_pGraphics->DrawImage(m_pTowerButtonImages[i], m_ButtonBufferRects[i]);
	}
}
