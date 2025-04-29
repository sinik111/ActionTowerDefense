#include "framework.h"
#include "GoldDisplay.h"

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "GameData.h"

GoldDisplay::GoldDisplay()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_LastGold(0),
	m_pImage(nullptr), m_TextSize(30)
{
	m_RenderLayer = RenderLayer::ScreenUI;
}

GoldDisplay::~GoldDisplay()
{
	if (m_pGraphics != nullptr)
	{
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}

	if (m_pBuffer != nullptr)
	{
		delete m_pBuffer;
		m_pBuffer = nullptr;
	}
}

void GoldDisplay::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"Gold");

	m_Text = std::to_wstring(GameData::Get().GetCurrentGold()) + L" G";

	m_TextPoint = Gdiplus::PointF(100.0f, 10.0f);

	m_PanelRect = Gdiplus::Rect(880, 0, 400, 70);

	m_GoldImagePosition = Gdiplus::Rect(25, 10, m_pImage->GetWidth(), m_pImage->GetHeight());

	m_pBuffer = new Gdiplus::Bitmap(m_PanelRect.Width, m_PanelRect.Height, PixelFormat24bppRGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	DrawOnBuffer();
}

void GoldDisplay::Destroy()
{
	__super::Destroy();
}

void GoldDisplay::Update()
{
	int gold = GameData::Get().GetCurrentGold();
	if (gold != m_LastGold)
	{
		m_LastGold = gold;

		m_Text = std::to_wstring(m_LastGold) + L" G";

		DrawOnBuffer();
	}

	if (!m_IsDestroyed)
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

void GoldDisplay::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawImage(m_pBuffer, m_PanelRect);
}

void GoldDisplay::DrawOnBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);

	GDIRenderer::Get().DrawString(m_pGraphics, m_Text.c_str(), Gdiplus::Color::Yellow, m_TextPoint, m_TextSize);
	m_pGraphics->DrawImage(m_pImage, m_GoldImagePosition);
}
