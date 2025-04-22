#include "framework.h"
#include "ScreenTextUI.h"

#include "GDIRenderer.h"
#include "RenderManager.h"

ScreenTextUI::ScreenTextUI(const std::wstring& text, const Vector2& position, const Gdiplus::Color& color, int size)
	: m_Text(text), m_Color(color), m_Size(size)
{
	m_Position = position;
	m_RenderLayer = RenderLayer::ScreenUI;
}

void ScreenTextUI::Initialize()
{

}

void ScreenTextUI::Destroy()
{
	__super::Destroy();
}

void ScreenTextUI::Update()
{
	__super::Update();
}

void ScreenTextUI::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawString(m_Text.c_str(), m_Color, m_Position, m_Size);
}
