#include "framework.h"
#include "TowerPlace.h"

#include "Constant.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TowerButton.h"

TowerPlace::TowerPlace(int row, int column)
	: m_pImage(nullptr), m_SrcRect(0, 0, TILE_SIZE, TILE_SIZE)
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

void TowerPlace::CreateTower(TowerType towerType)
{
}

void TowerPlace::UpgradeTower(TowerType towerType)
{
}
