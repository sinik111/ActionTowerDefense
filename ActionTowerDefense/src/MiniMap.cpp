#include "framework.h"
#include "MiniMap.h"

#include <sstream>

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Debug.h"
#include "SceneManager.h"

MiniMap::MiniMap()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_Rows(0), m_Columns(0)
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
Gdiplus::Rect dstRect;
Gdiplus::Rect srcRect;
int startPositionX;
int startPositionY;
void MiniMap::Initialize()
{


	m_RenderLayer = RenderLayer::ScreenUI;

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



	srcRect.X = 0;
	srcRect.Y = 0;
	srcRect.Width = m_TileImages[0]->GetWidth();
	srcRect.Height = m_TileImages[0]->GetWidth();

	dstRect.Width = srcRect.Width;
	dstRect.Height = srcRect.Height;

	int cameraWidth = SceneManager::Get().GetCurrentCamera()->GetWidth();
	int cameraHeight = SceneManager::Get().GetCurrentCamera()->GetHeight();

	/*int */startPositionX = cameraWidth - m_Rows * srcRect.Width;
	/*int */startPositionY = cameraHeight - m_Columns * srcRect.Height;

	GDIRenderer::Get().DrawRectangle(Gdiplus::Color(1, 1, 1),
		Gdiplus::Rect(startPositionX - 10, startPositionY - 10,
			cameraWidth - (startPositionX - 10),
			cameraWidth - (startPositionY - 10)));



	//int drawCount = 0;
	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		Gdiplus::Bitmap* image = m_TileImages[m_Tiles[i]];

		//dstRect.X = startPositionX + (i % m_Rows * srcRect.Width);
		//dstRect.Y = startPositionY + (i / m_Rows * srcRect.Height);

		dstRect.X = i % m_Rows * srcRect.Width;
		dstRect.Y = i / m_Rows * srcRect.Height;

		m_pGraphics->DrawImage(image, dstRect.X, dstRect.Y, srcRect.Width, srcRect.Height);
		//++drawCount;
		//GDIRenderer::Get().DrawImage(image, dstRect, srcRect);
	}
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


	//Debug::Log(std::to_string(drawCount));

	GDIRenderer::Get().DrawImage(m_pBuffer,
		Gdiplus::Rect(startPositionX, startPositionY, m_Rows * srcRect.Width, m_Columns * srcRect.Height),
		Gdiplus::Rect(0, 0, m_Rows * srcRect.Width, m_Columns * srcRect.Height));
}
