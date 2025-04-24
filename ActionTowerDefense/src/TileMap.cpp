#include "framework.h"
#include "TileMap.h"

#include <sstream>
#include <cmath>

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Debug.h"
#include "Constant.h"

TileMap::TileMap()
	: m_Rows(0), m_Columns(0)
{
}

void TileMap::Initialize()
{
	m_Position = Vector2::Zero;

	m_RenderLayer = RenderLayer::Background;

	m_SrcRect = Gdiplus::Rect(0, 0, TILE_SIZE, TILE_SIZE);

	m_Rows = MAP_SIZE;
	m_Columns = MAP_SIZE;

	const std::wstring& data = ResourceManager::Get().GetString(L"Play", L"TileMapData");

	std::wstringstream wss(data);

	m_Tiles.resize(m_Rows * m_Columns);

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		wss >> m_Tiles[i];
	}

	m_TileImages.resize(3);

	m_TileImages[0] = ResourceManager::Get().GetImage(L"Play", L"Tile0");
	m_TileImages[1] = ResourceManager::Get().GetImage(L"Play", L"Tile1");
	m_TileImages[2] = ResourceManager::Get().GetImage(L"Play", L"Tile2");
}

void TileMap::Destroy()
{
	__super::Destroy();
}

void TileMap::Update()
{
	__super::Update();
}

void TileMap::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect;

	dstRect.Width = m_SrcRect.Width;
	dstRect.Height = m_SrcRect.Height;

	Vector2 cameraPosition = camera.GetPosition();
	Vector2 cameraViewPos = camera.ToCameraView(m_Position);
	int cameraWidth = camera.GetWidth();
	int cameraHeight = camera.GetHeight();

	int startCol = max((int)(cameraPosition.x / m_SrcRect.Width), 0);
	int endCol = min((int)((cameraPosition.x + cameraWidth) / m_SrcRect.Width) + 1, m_Columns);

	int startRow = max((int)(cameraPosition.y / m_SrcRect.Height), 0);
	int endRow = min((int)((cameraPosition.y + cameraHeight) / m_SrcRect.Height) + 1, m_Rows);

	for (int row = startRow; row < endRow; ++row)
	{
		for (int col = startCol; col < endCol; ++col)
		{
			int i = row * m_Columns + col;
			Gdiplus::Bitmap* image = m_TileImages[m_Tiles[i]];

			dstRect.X = col * m_SrcRect.Width + (int)cameraViewPos.x;
			dstRect.Y = row * m_SrcRect.Height + (int)cameraViewPos.y;


			GDIRenderer::Get().DrawImage(image, dstRect, m_SrcRect);
		}
	}
}
