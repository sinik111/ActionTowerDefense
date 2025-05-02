#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class TowerPlace;
class Camera;

class TowerButton :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImages[2];
	std::wstring m_Text;
	TowerPlace* m_pTargetTowerPlace;
	Gdiplus::Rect m_SrcRect;
	int m_CurrentImageIndex;
	bool m_IsButtonDown;

	bool m_IsBuildButtonCreated;

public:
	TowerButton(TowerPlace* pTargetTowerPlace);
	~TowerButton() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};