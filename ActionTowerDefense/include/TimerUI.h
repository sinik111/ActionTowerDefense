#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class TimerUI :
	public Object
{
private:
	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;

	Gdiplus::Rect m_PanelRect;

	std::wstring m_Text;
	Gdiplus::PointF m_TextPoint;
	int m_TextSize;
	int m_CurrentTime;

public:
	TimerUI();
	~TimerUI();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

private:
	void DrawOnBuffer();
};