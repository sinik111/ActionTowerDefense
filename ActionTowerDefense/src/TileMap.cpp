#include "framework.h"
#include "TileMap.h"

#include <sstream>

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Debug.h"

void TileMap::Initialize()
{
	m_Position = Vector2::Zero;

	m_RenderLayer = RenderLayer::Background;

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

	m_TileImages[0] = ResourceManager::Get().GetImage(L"Play", L"Tile0");
	m_TileImages[1] = ResourceManager::Get().GetImage(L"Play", L"Tile1");
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
	Gdiplus::Rect srcRect;

	srcRect.X = 0;
	srcRect.Y = 0;
	srcRect.Width = m_TileImages[0]->GetWidth();
	srcRect.Height = m_TileImages[0]->GetWidth();

	dstRect.Width = srcRect.Width;
	dstRect.Height = srcRect.Height;

	Vector2 cameraPosition = camera.GetPosition();
	Vector2 cameraViewPos = camera.ToCameraView(m_Position);
	int cameraWidth = camera.GetWidth();
	int cameraHeight = camera.GetHeight();

	//Gdiplus::Bitmap* buffer = new Gdiplus::Bitmap(cameraWidth, cameraHeight, PixelFormat32bppARGB);
	//Gdiplus::Graphics g(buffer);

	//Debug::Log(cameraViewPos.ToString());

	//int drawCount = 0;

	for (int i = 0; i < m_Rows * m_Columns; ++i)
	{

		if (cameraPosition.x + cameraWidth < i % m_Rows * srcRect.Width)
		{
			continue;
		}

		if (cameraPosition.x > (i + 1) % m_Rows * srcRect.Width)
		{
			continue;
		}

		if (cameraPosition.y + cameraHeight < i / m_Rows * srcRect.Height)
		{
			continue;
		}

		if (cameraPosition.y > (i / m_Rows + 1) * srcRect.Height)
		{
			continue;
		}

		Gdiplus::Bitmap* image = m_TileImages[m_Tiles[i]];

		dstRect.X = (i % m_Rows * srcRect.Width) + (int)cameraViewPos.x;
		dstRect.Y = (i / m_Rows * srcRect.Height) + (int)cameraViewPos.y;

		//Debug::Log(std::to_string(dstRect.X));
		//Debug::Log(std::to_string(dstRect.Y));

		//g.DrawImage(image, dstRect.X, dstRect.Y, srcRect.Width, srcRect.Height);
		//++drawCount;
		GDIRenderer::Get().DrawImage(image, dstRect, srcRect);
	}

	//Debug::Log(std::to_string(drawCount));

	//GDIRenderer::Get().DrawImage(buffer, 
	//	Gdiplus::Rect(0, 0, cameraWidth, cameraHeight),
	//	Gdiplus::Rect(0, 0, cameraWidth, cameraHeight));

	//delete buffer;
}
