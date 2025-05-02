#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class Camera;
enum class ResultCode;

enum class TileType
{
	Field,
	Path,
	TowerPlace,
	Gate,
	Crystal,
	Max
};

class TileMap :
	public Object
{
private:
	Gdiplus::Color m_TileColors[(int)TileType::Max];
	std::vector<char> m_Tiles;
	int m_Rows;
	int m_Columns;
	int m_Level;
	int m_GateCounter;

public:
	TileMap();
	~TileMap() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	void SetLevel(int level);

private:
	void SetMap();
};