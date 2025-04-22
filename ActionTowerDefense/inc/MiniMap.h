#pragma once

#include "Object.h"
#include "GDIRenderer.h"

namespace Gdiplus
{
	class Bitmap;
	class Graphics;
}

class Camera;
enum class ResultCode;

class MiniMap :
	public Object
{
private:
	Gdiplus::Bitmap* m_pMiniMapPlayer;
	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;
	std::vector<Gdiplus::Bitmap*> m_TileImages;
	std::vector<short> m_Tiles;
	Gdiplus::Rect m_DstRect;
	Gdiplus::Rect m_SrcRect;
	int m_Rows;
	int m_Columns;
	int m_TileSize;
	Vector2 m_StartPosition;

public:
	MiniMap();
	~MiniMap();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

private:
	void ReDrawMiniMap() const;
};