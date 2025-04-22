#include "framework.h"
#include "MiniMap.h"

#include <sstream>

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Debug.h"

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
	Gdiplus::Rect dstRect;
	Gdiplus::Rect srcRect;

	srcRect.X = 0;
	srcRect.Y = 0;
	srcRect.Width = m_TileImages[0]->GetWidth();
	srcRect.Height = m_TileImages[0]->GetWidth();

	dstRect.Width = srcRect.Width;
	dstRect.Height = srcRect.Height;

	int cameraWidth = camera.GetWidth();
	int cameraHeight = camera.GetHeight();

	int startPositionX = cameraWidth - m_Rows * srcRect.Width;
	int startPositionY = cameraHeight - m_Columns * srcRect.Height;

	GDIRenderer::Get().DrawRectangle(Gdiplus::Color(1, 1, 1),
		Gdiplus::Rect(startPositionX - 10, startPositionY - 10,
			cameraWidth - (startPositionX - 10),
			cameraWidth - (startPositionY - 10)));

	Gdiplus::Bitmap* buffer = new Gdiplus::Bitmap(m_Rows * srcRect.Width, m_Columns * srcRect.Height,
		PixelFormat32bppARGB);

	Gdiplus::Graphics g(buffer);

	//int drawCount = 0;
	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{
		Gdiplus::Bitmap* image = m_TileImages[m_Tiles[i]];

		//dstRect.X = startPositionX + (i % m_Rows * srcRect.Width);
		//dstRect.Y = startPositionY + (i / m_Rows * srcRect.Height);

		dstRect.X = i % m_Rows * srcRect.Width;
		dstRect.Y = i / m_Rows * srcRect.Height;

		g.DrawImage(image, dstRect.X, dstRect.Y, srcRect.Width, srcRect.Height);
		//++drawCount;
		//GDIRenderer::Get().DrawImage(image, dstRect, srcRect);
	}

	//Debug::Log(std::to_string(drawCount));

	GDIRenderer::Get().DrawImage(buffer,
		Gdiplus::Rect(startPositionX, startPositionY, m_Rows * srcRect.Width, m_Columns * srcRect.Height),
		Gdiplus::Rect(0, 0, m_Rows * srcRect.Width, m_Columns * srcRect.Height));

	delete buffer;
}
