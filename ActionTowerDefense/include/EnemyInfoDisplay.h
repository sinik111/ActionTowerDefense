#pragma once

#include "Object.h"
#include "GDIRenderer.h"
#include "GameData.h"
#include "Enemy.h"

class EnemyInfoDisplay :
	public Object
{
private:
	Gdiplus::Color m_Colors[(int)EnemyType::Max];
	Gdiplus::Bitmap* m_pBuffer;
	Gdiplus::Graphics* m_pGraphics;

	Gdiplus::Rect m_PanelRect;
	Gdiplus::Rect m_ImageDstRect;

	std::wstring m_TypeName;
	std::wstring m_InfoText;

	Gdiplus::PointF m_NameTextPoint;
	Gdiplus::PointF m_InfoTextPoint;
	Gdiplus::Point m_HpStartPoint;
	Gdiplus::Point m_HpEndPoint;

	EnemyInfo m_EnemyInfo;

	int m_NameTextSize;
	int m_InfoTextSize;
	bool m_HasEnemyInfo;

public:
	EnemyInfoDisplay();
	~EnemyInfoDisplay();

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void LateUpdate() override;
	void Render(const Camera& camera) const override;

private:
	void DrawOnBuffer();
	void ClearBuffer();
};