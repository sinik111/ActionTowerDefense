#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class Camera;
enum class ResultCode;

class TileMap :
	public Object
{
private:
	std::vector<Gdiplus::Bitmap*> m_TileImages;
	std::vector<short> m_Tiles;
	Gdiplus::Rect m_SrcRect;
	int m_Rows;
	int m_Columns;

public:
	TileMap();
	~TileMap() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};