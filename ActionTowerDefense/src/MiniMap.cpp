#include "framework.h"
#include "MiniMap.h"

#include <sstream>

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Debug.h"
#include "SceneManager.h"
#include "GameData.h"
#include "Constant.h"
#include "Input.h"

MiniMap::MiniMap()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_Rows(0), m_Columns(0),
	m_TileSize(0), m_StartPosition(Vector2::Zero), m_Level(1)
{
}

MiniMap::~MiniMap()
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

void MiniMap::Initialize()
{
	m_RenderLayer = RenderLayer::ScreenUI;

	m_ObjectColors[(int)MiniMapObjectType::Player] = Gdiplus::Color(34, 177, 76);
	m_ObjectColors[(int)MiniMapObjectType::Enemy] = Gdiplus::Color(255, 0, 0);
	m_ObjectColors[(int)MiniMapObjectType::Tower] = Gdiplus::Color(255, 255, 0);

	m_Rows = MAP_SIZE;
	m_Columns = MAP_SIZE;

	m_Tiles.resize(m_Rows * m_Columns);

	m_TileColors[(int)TileType::Field] = Gdiplus::Color(239, 228, 176);
	m_TileColors[(int)TileType::Path] = Gdiplus::Color(185, 122, 87);
	m_TileColors[(int)TileType::TowerPlace] = Gdiplus::Color(163, 73, 164);
	m_TileColors[(int)TileType::Gate] = Gdiplus::Color(0, 162, 232);
	m_TileColors[(int)TileType::Crystal] = Gdiplus::Color(194, 255, 192);

	m_TileSize = MINI_TILE_SIZE;


	int cameraWidth = SceneManager::Get().GetCurrentCamera()->GetWidth();
	int cameraHeight = SceneManager::Get().GetCurrentCamera()->GetHeight();

	m_StartPosition.x = (float)(cameraWidth - (m_Rows + 1) * m_TileSize);
	m_StartPosition.y = (float)(cameraHeight - (m_Columns + 1) * m_TileSize);

	m_PanelRect = Gdiplus::Rect(
		cameraWidth - (m_Rows + 2) * m_TileSize, cameraHeight - (m_Columns + 2) * m_TileSize,
		(m_Rows + 2) * m_TileSize, (m_Columns + 2) * m_TileSize);

	m_pBuffer = new Gdiplus::Bitmap(m_PanelRect.Width, m_PanelRect.Height, PixelFormat24bppRGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	SetMinimap();
}

void MiniMap::Destroy()
{
	__super::Destroy();
}

void MiniMap::Update()
{
	if (m_Level == 1)
	{
		if (GameData::Get().GetRemainPlayTime() < 240)
		{
			SetLevel(2);
		}
	}
	else if (m_Level == 2)
	{
		if (GameData::Get().GetRemainPlayTime() < 180)
		{
			SetLevel(3);
		}
	}

	/*if (Input::IsKeyReleased('2'))
	{
		SetLevel(2);
	}

	if (Input::IsKeyReleased('3'))
	{
		SetLevel(3);
	}*/

	__super::Update();
}

void MiniMap::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawImage(m_pBuffer, m_PanelRect);

	const std::vector<MiniMapInfo>& infos = GameData::Get().GetMiniMapInfo();

	for (auto& info : infos)
	{
		float scale = (float)MINI_TILE_SIZE / TILE_SIZE; // 미니맵 타일 / 월드 타일

		Vector2 miniPosition = m_StartPosition + info.position * scale;

		Gdiplus::Rect dstRect(
			(int)miniPosition.x - MINI_MAP_OBJECT_SIZE / 2,
			(int)miniPosition.y - MINI_MAP_OBJECT_SIZE / 2,
			MINI_MAP_OBJECT_SIZE, MINI_MAP_OBJECT_SIZE);

		switch (info.type)
		{
		case MiniMapObjectType::Player:
			GDIRenderer::Get().DrawFillRectangle(m_ObjectColors[(int)MiniMapObjectType::Player], dstRect);
			break;

		case MiniMapObjectType::Enemy:
			GDIRenderer::Get().DrawFillRectangle(m_ObjectColors[(int)MiniMapObjectType::Enemy], dstRect);
			break;

		case MiniMapObjectType::Tower:
			GDIRenderer::Get().DrawFillRectangle(m_ObjectColors[(int)MiniMapObjectType::Tower], dstRect);
			break;
		}
	}

	GameData::Get().ClearMiniMapInfo();
}

void MiniMap::SetLevel(int level)
{
	m_Level = level;
	SetMinimap();
}

void MiniMap::ReDrawMiniMap() const
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		Gdiplus::Rect dstRect(
			(int)i % m_Rows * m_TileSize + m_TileSize,
			(int)i / m_Rows * m_TileSize + m_TileSize,
			m_TileSize, m_TileSize);

		GDIRenderer::Get().DrawFillRectangle(m_pGraphics, m_TileColors[m_Tiles[i]], dstRect);
	}
}

void MiniMap::SetMinimap()
{
	const std::wstring& data = ResourceManager::Get().GetString(L"Play", L"TileMapData" + std::to_wstring(m_Level));

	std::wstringstream wss(data);

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		int tileNumber;
		wss >> tileNumber;

		m_Tiles[i] = tileNumber;
	}

	ReDrawMiniMap();
}
