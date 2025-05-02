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
#include "Scene.h"
#include "SceneManager.h"
#include "TowerPlace.h"
#include "EnterGate.h"

TileMap::TileMap()
	: m_Rows(0), m_Columns(0), m_Level(1), m_GateCounter(1)
{
}

void TileMap::Initialize()
{
	m_Position = Vector2::Zero;

	m_RenderLayer = RenderLayer::Background;

	m_Rows = MAP_SIZE;
	m_Columns = MAP_SIZE;

	m_Tiles.resize(m_Rows * m_Columns);

	m_TileColors[(int)TileType::Field] = Gdiplus::Color(239, 228, 176);
	m_TileColors[(int)TileType::Path] = Gdiplus::Color(185, 122, 87);
	m_TileColors[(int)TileType::TowerPlace] = Gdiplus::Color(163, 73, 164);
	m_TileColors[(int)TileType::Gate] = Gdiplus::Color(0, 162, 232);
	m_TileColors[(int)TileType::Crystal] = Gdiplus::Color(194, 255, 192);
	
	SetMap();
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

	dstRect.Width = TILE_SIZE;
	dstRect.Height = TILE_SIZE;

	Vector2 cameraPosition = camera.GetPosition();
	Vector2 cameraViewPos = camera.ToCameraView(m_Position);
	int cameraWidth = camera.GetWidth();
	int cameraHeight = camera.GetHeight();

	int startCol = max((int)(cameraPosition.x / TILE_SIZE), 0);
	int endCol = min((int)((cameraPosition.x + cameraWidth) / TILE_SIZE) + 1, m_Columns);

	int startRow = max((int)(cameraPosition.y / TILE_SIZE), 0);
	int endRow = min((int)((cameraPosition.y + cameraHeight) / TILE_SIZE) + 1, m_Rows);

	for (int row = startRow; row < endRow; ++row)
	{
		for (int col = startCol; col < endCol; ++col)
		{
			int i = row * m_Columns + col;

			dstRect.X = col * TILE_SIZE + (int)cameraViewPos.x;
			dstRect.Y = row * TILE_SIZE + (int)cameraViewPos.y;

			GDIRenderer::Get().DrawFillRectangle(m_TileColors[m_Tiles[i]], dstRect);
		}
	}
}

void TileMap::SetLevel(int level)
{
	m_Level = level;

	SetMap();
}

void TileMap::SetMap()
{
	const std::wstring& data = ResourceManager::Get().GetString(L"Play", L"TileMapData" + std::to_wstring(m_Level));

	std::wstringstream wss(data);

	Scene* pScene = SceneManager::Get().GetCurrentScene();

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		int tileType;

		wss >> tileType;
		
		if (m_Level == 1)
		{
			m_Tiles[i] = tileType;

			switch (tileType)
			{
			case (int)TileType::TowerPlace:
				pScene->CreateObject<TowerPlace>(i % MAP_SIZE, i / MAP_SIZE);
				break;

			case (int)TileType::Gate:
				pScene->CreateObject<EnterGate>(m_GateCounter++, i % MAP_SIZE, i / MAP_SIZE);
				break;
			}
		}
		else
		{
			if (m_Tiles[i] != tileType)
			{
				switch (tileType)
				{
				case (int)TileType::TowerPlace:
					pScene->CreatePendingObject<TowerPlace>(i % MAP_SIZE, i / MAP_SIZE);
					break;

				case (int)TileType::Gate:
					pScene->CreatePendingObject<EnterGate>(m_GateCounter++, i % MAP_SIZE, i / MAP_SIZE);
					break;
				}
			}

			m_Tiles[i] = tileType;
		}
	}
}
