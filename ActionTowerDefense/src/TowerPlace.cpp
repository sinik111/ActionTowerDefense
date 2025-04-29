#include "framework.h"
#include "TowerPlace.h"

#include "Constant.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TowerButton.h"
#include "FireTower.h"
#include "IceTower.h"
#include "LightningTower.h"
#include "GameData.h"
#include "ScreenTextUI.h"

TowerPlace::TowerPlace(int row, int column)
	: m_pImage(nullptr), m_SrcRect(0, 0, TILE_SIZE, TILE_SIZE), m_pTower(nullptr)
{
	m_Position = Vector2(TILE_SIZE * row + (float)TILE_SIZE / 2,
		TILE_SIZE * column + (float)TILE_SIZE / 2);

	m_RenderLayer = RenderLayer::Object;
}

void TowerPlace::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"TowerPlace");

	SceneManager::Get().GetCurrentScene()->CreateObject<TowerButton>(this);
}

void TowerPlace::Destroy()
{
	__super::Destroy();
}

void TowerPlace::Update()
{
	if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position, m_SrcRect.Width, m_SrcRect.Height))
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

void TowerPlace::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect;
	dstRect.Width = m_SrcRect.Width;
	dstRect.Height = m_SrcRect.Height;

	Vector2 cameraViewPosition = camera.ToCameraView(m_Position);

	dstRect.X = (int)(cameraViewPosition.x - m_SrcRect.Width / 2);
	dstRect.Y = (int)(cameraViewPosition.y - m_SrcRect.Height / 2);

	GDIRenderer::Get().DrawImage(m_pImage, dstRect, m_SrcRect);
}

TowerState TowerPlace::GetTowerState()
{
	return m_TowerState;
}

void TowerPlace::UpgradeTower(TowerType towerType)
{
	if (m_TowerState.type == TowerType::MAX || m_TowerState.type != towerType)
	{
		if (!GameData::Get().UseGold(100))
		{
			SceneManager::Get().GetCurrentScene()->CreatePendingObject<ScreenTextUI>(
				L"골드가 부족합니다", Vector2(540.0f, 400.0f), Gdiplus::Color::Red, 24, 2.0f);

			return;
		}

		if (m_TowerState.type != TowerType::MAX && m_TowerState.type != towerType)
		{
			m_pTower->Destroy();
		}

		switch (towerType)
		{
		case TowerType::Fire:
			m_pTower = SceneManager::Get().GetCurrentScene()->CreatePendingObject<FireTower>(m_Position);
			break;
		case TowerType::Ice:
			m_pTower = SceneManager::Get().GetCurrentScene()->CreatePendingObject<IceTower>(m_Position);
			break;
		case TowerType::Lightning:
			m_pTower = SceneManager::Get().GetCurrentScene()->CreatePendingObject<LightningTower>(m_Position);
			break;
		}

		m_TowerState.type = towerType;
		m_TowerState.level = 1;
	}
	else if (m_TowerState.type == towerType)
	{
		if (m_TowerState.level == 3)
		{
			return;
		}

		if (!GameData::Get().UseGold(100))
		{
			SceneManager::Get().GetCurrentScene()->CreatePendingObject<ScreenTextUI>(
				L"골드가 부족합니다", Vector2(540.0f, 400.0f), Gdiplus::Color::Red, 24, 2.0f);

			return;
		}

		m_pTower->Upgrade();

		++m_TowerState.level;
	}
}
