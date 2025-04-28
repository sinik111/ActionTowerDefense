#pragma once

#include "Object.h"
#include "GDIRenderer.h"

enum class TowerType
{
	Fire,
	Ice,
	Lightning,
	MAX
};

struct TowerState
{
	TowerType type = TowerType::MAX;
	int level = 0;
};

class Tower;

class TowerPlace :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImage;
	Gdiplus::Rect m_SrcRect;

	TowerState m_TowerState;
	Tower* m_pTower;

public:
	TowerPlace(int row, int column);
	~TowerPlace() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	TowerState GetTowerState();

	void UpgradeTower(TowerType towerType);
};