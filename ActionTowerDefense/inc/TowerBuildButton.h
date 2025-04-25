#pragma once

#include "Object.h"
#include "GDIRenderer.h"

#include "TowerPlace.h"

class Camera;

class TowerBuildButton :
	public Object
{
private:
	Gdiplus::Bitmap* m_pTowerButtonImages[(int)TowerType::MAX];
	TowerPlace* m_pTargetTowerPlace;

	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;

	Gdiplus::Rect m_PanelRect;
	Gdiplus::Rect m_ButtonBufferRects[(int)TowerType::MAX];
	Collider m_ButtonCollider[(int)TowerType::MAX];
	bool m_IsButtonDown[(int)TowerType::MAX];

public:
	TowerBuildButton(TowerPlace* pTargetTowerPlace);
	~TowerBuildButton();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

private:
	void LoadTowerButtonImages();
	void DrawOnBuffer();
};