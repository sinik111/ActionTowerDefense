#pragma once

#include "Object.h"

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
	std::vector<Gdiplus::Bitmap*> m_TileImages;
	std::vector<short> m_Tiles;
	int m_Rows;
	int m_Columns;

public:
	MiniMap();
	~MiniMap();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};