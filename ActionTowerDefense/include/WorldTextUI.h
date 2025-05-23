#pragma once

#include "Object.h"
#include "GDIRenderer.h"

class WorldTextUI :
	public Object
{
private:
	std::wstring m_Text;
	Gdiplus::Color m_Color;
	int m_Size;

public:
	WorldTextUI(const std::wstring& text, const Vector2& position, const Gdiplus::Color& color, int size);
	~WorldTextUI() = default;

public:
	virtual void Initialize();
	virtual void Destroy();

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};