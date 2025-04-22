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

MiniMap::MiniMap()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_Rows(0), m_Columns(0),
	m_TileSize(0), m_pMiniMapPlayer(nullptr), m_StartPosition(Vector2::Zero)
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

	const std::wstring& data = ResourceManager::Get().GetString(L"Play", L"TileMapData");

	std::wstringstream wss(data);

	wss >> m_Rows;
	wss >> m_Columns;

	m_Tiles.resize(m_Rows * m_Columns);

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		wss >> m_Tiles[i];
	}

	m_TileImages.resize(2);

	m_TileImages[0] = ResourceManager::Get().GetImage(L"Play", L"MiniTile0");
	m_TileImages[1] = ResourceManager::Get().GetImage(L"Play", L"MiniTile1");

	m_pBuffer = new Gdiplus::Bitmap(m_Rows * m_TileImages[0]->GetWidth(),
		m_Columns * m_TileImages[0]->GetHeight(),
		PixelFormat32bppARGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	m_TileSize = m_TileImages[0]->GetWidth();
	
	Gdiplus::Rect dstRect;
	Gdiplus::Rect srcRect;

	srcRect.X = 0;
	srcRect.Y = 0;
	srcRect.Width = m_TileSize;
	srcRect.Height = m_TileSize;

	dstRect.Width = m_TileSize;
	dstRect.Height = m_TileSize;

	int cameraWidth = SceneManager::Get().GetCurrentCamera()->GetWidth();
	int cameraHeight = SceneManager::Get().GetCurrentCamera()->GetHeight();

	m_StartPosition.x = (float)(cameraWidth - m_Rows * m_TileSize);
	m_StartPosition.y = (float)(cameraHeight - m_Columns * m_TileSize);

	ReDrawMiniMap();

	m_DstRect = Gdiplus::Rect((int)m_StartPosition.x, (int)m_StartPosition.y, m_Rows * m_TileSize, m_Columns * m_TileSize);
	m_SrcRect = Gdiplus::Rect(0, 0, m_Rows * m_TileSize, m_Columns * m_TileSize);
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
	GDIRenderer::Get().DrawImage(m_pBuffer, m_DstRect, m_SrcRect);

	const std::vector<MiniMapInfo>& infos = GameData::Get().GetMiniMapInfo();

	for (auto& info : infos)
	{
		switch (info.type)
		{
		case MiniMapObjectType::Player:
			float scale = 8.0f / 128.0f; // 미니맵 타일 / 월드 타일

			Vector2 miniPosition = m_StartPosition + info.position * scale;

			Gdiplus::Rect dstRect;
			dstRect.X = (int)miniPosition.x - m_pMiniMapPlayer->GetWidth() / 2;
			dstRect.Y = (int)miniPosition.y - m_pMiniMapPlayer->GetHeight() / 2;
			dstRect.Width = m_pMiniMapPlayer->GetWidth();
			dstRect.Height = m_pMiniMapPlayer->GetHeight();

			GDIRenderer::Get().DrawImage(m_pMiniMapPlayer, dstRect,
				Gdiplus::Rect(0, 0, m_pMiniMapPlayer->GetWidth(), m_pMiniMapPlayer->GetHeight()));
			break;
		}
	}

	GameData::Get().ClearMiniMapInfo();
}

void MiniMap::ReDrawMiniMap() const
{
	m_pGraphics->Clear(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		Gdiplus::Bitmap* image = m_TileImages[m_Tiles[i]];

		int posX = i % m_Rows * m_TileSize;
		int posY = i / m_Rows * m_TileSize;

		m_pGraphics->DrawImage(image, posX, posY, m_TileSize, m_TileSize);
	}
}
