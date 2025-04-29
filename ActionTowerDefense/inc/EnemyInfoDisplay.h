#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class EnemyInfoDisplay :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImages[3];
	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;

	Gdiplus::Rect m_PanelRect;

	std::wstring m_TypeName;
	std::wstring m_Hp;
	std::wstring m_MaxHp;
	std::wstring m_State;

	Gdiplus::PointF m_NameTextPoint;
	Gdiplus::PointF m_HpTextPoint;
	Gdiplus::PointF m_StateTextPoint;

	int m_TextSize;

	Gdiplus::Rect m_GoldImagePosition;

	int m_LastGold;

public:
	EnemyInfoDisplay();
	~EnemyInfoDisplay();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

private:
	void DrawOnBuffer();
};