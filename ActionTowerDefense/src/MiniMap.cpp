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

MiniMap::MiniMap()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_Rows(0), m_Columns(0),
	m_TileSize(0), m_pMiniMapPlayer(nullptr), m_StartPosition(Vector2::Zero),
	m_pMiniMapEnterGate(nullptr), m_pMiniMapEnemy(nullptr)
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

	m_pMiniMapPlayer = ResourceManager::Get().GetImage(L"Play", L"MiniMapPlayer");
	m_pMiniMapEnterGate = ResourceManager::Get().GetImage(L"Play", L"MiniMapEnterGate");
	m_pMiniMapEnemy = ResourceManager::Get().GetImage(L"Play", L"MiniMapEnemy");

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

	m_TileImages[0] = ResourceManager::Get().GetImage(L"Play", L"MiniTile0");
	m_TileImages[1] = ResourceManager::Get().GetImage(L"Play", L"MiniTile1");
	m_TileImages[2] = ResourceManager::Get().GetImage(L"Play", L"MiniTile2");

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

	ReDrawMiniMap();
}

void MiniMap::Destroy()
{
	__super::Destroy();
}

void MiniMap::Update()
{
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

		Gdiplus::Bitmap* current = nullptr;
		switch (info.type)
		{
		case MiniMapObjectType::Player:
			current = m_pMiniMapPlayer;
			break;

		case MiniMapObjectType::EnterGate:
			current = m_pMiniMapEnterGate;
			break;

		case MiniMapObjectType::Enemy:
			current = m_pMiniMapEnemy;
			break;
		}

		Gdiplus::Rect srcRect(0, 0, current->GetWidth(), current->GetHeight());
		Gdiplus::Rect dstRect(
			(int)miniPosition.x - srcRect.Width / 2,
			(int)miniPosition.y - srcRect.Height / 2,
			srcRect.Width, srcRect.Height);

		GDIRenderer::Get().DrawImage(current, dstRect, srcRect);
	}

	GameData::Get().ClearMiniMapInfo();
}

void MiniMap::ReDrawMiniMap() const
{
	m_pGraphics->Clear(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		Gdiplus::Bitmap* image = m_TileImages[m_Tiles[i]];

		int posX = i % m_Rows * m_TileSize + m_TileSize;
		int posY = i / m_Rows * m_TileSize + m_TileSize;

		m_pGraphics->DrawImage(image, posX, posY, m_TileSize, m_TileSize);
	}
}
