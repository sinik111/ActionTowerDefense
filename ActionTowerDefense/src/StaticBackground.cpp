#include "framework.h"
#include "StaticBackground.h"

#include "GDIRenderer.h"
#include "RenderManager.h"

StaticBackground::StaticBackground(Gdiplus::Bitmap* image)
	: m_Image(image), m_Width(0), m_Height(0)
{
	m_RenderLayer = RenderLayer::Background;
}

void StaticBackground::Initialize()
{
	m_Width = m_Image->GetWidth();
	m_Height = m_Image->GetHeight();
}

void StaticBackground::Destroy()
{
	__super::Destroy();
}

void StaticBackground::Update()
{
	__super::Update();
}

void StaticBackground::Render() const
{
	//GDIRenderer
}
