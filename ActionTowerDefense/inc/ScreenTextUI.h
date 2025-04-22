#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class Camera;

class ScreenTextUI :
	public Object
{
private:
	std::wstring m_Text;
	Gdiplus::Color m_Color;
	int m_Size;

public:
	ScreenTextUI(const std::wstring& text, const Vector2& position, const Gdiplus::Color& color, int size);
	~ScreenTextUI() = default;

public:
	virtual void Initialize();
	virtual void Destroy();

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};