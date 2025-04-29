#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class GoldDisplay :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImage;
	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;

	Gdiplus::Rect m_PanelRect;

	std::wstring m_Text;
	Gdiplus::PointF m_TextPoint;
	int m_TextSize;

	Gdiplus::Rect m_GoldImagePosition;

	int m_LastGold;

public:
	GoldDisplay();
	~GoldDisplay();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

private:
	void DrawOnBuffer();
};