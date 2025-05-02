#pragma once

#include "Object.h"
#include "GDIRenderer.h"
#include "GameData.h"
#include "TileMap.h"

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
	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;
	Gdiplus::Color m_ObjectColors[(int)MiniMapObjectType::Max];
	Gdiplus::Color m_TileColors[(int)TileType::Max];
	std::vector<char> m_Tiles;
	Gdiplus::Rect m_PanelRect;
	int m_Rows;
	int m_Columns;
	int m_TileSize;
	Vector2 m_StartPosition;

	int m_Level;

public:
	MiniMap();
	~MiniMap();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	void SetLevel(int level);

private:
	void ReDrawMiniMap() const;
	void SetMinimap();
};