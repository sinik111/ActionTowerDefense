#pragma once

#include "Object.h"

namespace Gdiplus
{
	class Bitmap;
}

class Camera;
enum class ResultCode;

class MiniMap :
	public Object
{
private:
	std::vector<Gdiplus::Bitmap*> m_TileImages;
	std::vector<short> m_Tiles;
	int m_Rows;
	int m_Columns;

public:
	MiniMap() = default;
	~MiniMap() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};