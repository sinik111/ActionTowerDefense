#include "framework.h"
#include "WorldTextUI.h"

#include "GDIRenderer.h"
#include "Camera.h"
#include "RenderManager.h"

WorldTextUI::WorldTextUI(const std::wstring& text, const Vector2& position, const Gdiplus::Color& color, float size)
	: m_Text(text), m_Color(color), m_Size(size)
{
	m_Position = position;
	m_RenderLayer = RenderLayer::WorldUI;
}

void WorldTextUI::Initialize()
{

}

void WorldTextUI::Destroy()
{
	__super::Destroy();
}

void WorldTextUI::Update()
{
	__super::Update();
}

void WorldTextUI::Render(const Camera& camera) const
{
	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	GDIRenderer::Get().DrawString(m_Text.c_str(), m_Color, cameraViewPos, m_Size);
}
