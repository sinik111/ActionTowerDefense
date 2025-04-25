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

	Collider collider;
	collider.SetColliderInfo(ColliderType::UIRect,
		Vector2((float)(m_PanelRect.X + 4), (float)(m_PanelRect.Y + 4)), Vector2::Zero, 256, 128);

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
				//TowerState towerState = m_pTargetTowerPlace->GetTowerState();

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

	if (towerState.type == TowerType::MAX)
	{
		m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play", L"FireTowerButtonIcon1");
		m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play", L"IceTowerButtonIcon1");
		m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play", L"LightningTowerButtonIcon1");
	}
	else if (towerState.level == 3)
	{
		switch (towerState.type)
		{
		case TowerType::Fire:
			m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play", L"TowerLevelMax");
			m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play", L"IceTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play", L"LightningTowerButtonIcon1");
			break;

		case TowerType::Ice:
			m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play", L"FireTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play", L"TowerLevelMax");
			m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play", L"LightningTowerButtonIcon1");
			break;

		case TowerType::Lightning:
			m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play", L"FireTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play", L"IceTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play", L"TowerLevelMax");
			break;
		}
	}
	else
	{
		switch (towerState.type)
		{
		case TowerType::Fire:
			m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play",
				L"FireTowerButtonIcon" + std::to_wstring(towerState.level + 1));
			m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play", L"IceTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play", L"LightningTowerButtonIcon1");
			break;

		case TowerType::Ice:
			m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play", L"FireTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play",
				L"IceTowerButtonIcon" + std::to_wstring(towerState.level + 1));
			m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play", L"LightningTowerButtonIcon1");
			break;

		case TowerType::Lightning:
			m_pTowerButtonImages[(int)TowerType::Fire] = ResourceManager::Get().GetImage(L"Play", L"FireTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Ice] = ResourceManager::Get().GetImage(L"Play", L"IceTowerButtonIcon1");
			m_pTowerButtonImages[(int)TowerType::Lightning] = ResourceManager::Get().GetImage(L"Play",
				L"LightningTowerButtonIcon" + std::to_wstring(towerState.level + 1));
			break;
		}
	}
}

void TowerBuildButton::DrawOnBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < (int)TowerType::MAX; ++i)
	{
		m_pGraphics->DrawImage(m_pTowerButtonImages[i], m_ButtonBufferRects[i]);
	}
}
