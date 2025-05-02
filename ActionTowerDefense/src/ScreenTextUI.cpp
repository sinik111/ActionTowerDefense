#include "framework.h"
#include "ScreenTextUI.h"

#include "GDIRenderer.h"
#include "RenderManager.h"
#include "MyTime.h"

ScreenTextUI::ScreenTextUI(const std::wstring& text, const Vector2& position,
	const Gdiplus::Color& color, int size, float timer)
	: m_Text(text), m_Color(color), m_Size(size), m_Timer(timer)
{
	if (timer < 0)
	{
		m_HasTimer = false;
	}
	else
	{
		m_HasTimer = true;
	}
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
	if (m_HasTimer)
	{
		m_Timer -= MyTime::DeltaTime();
		if (m_Timer < 0)
		{
			Destroy();
		}
	}

	__super::Update();
}

void ScreenTextUI::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawString(m_Text.c_str(), m_Color,
		Gdiplus::PointF(m_Position.x, m_Position.y), m_Size);
}
