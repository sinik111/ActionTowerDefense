#include "framework.h"
#include "CrystalHp.h"

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "GameData.h"
#include "CenterCrystal.h"

CrystalHp::CrystalHp()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_LastHp(5),
	m_HpPositions{}, m_pImage(nullptr), m_TextSize(30)
{
	m_RenderLayer = RenderLayer::ScreenUI;
}

CrystalHp::~CrystalHp()
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

void CrystalHp::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"CrystalHp");

	m_Text = L"Crystals";

	m_TextPoint = Gdiplus::PointF(10.0f, 10.0f);

	m_PanelRect = Gdiplus::Rect(0, 0, 400, 70);

	m_HpPositions[0] = Gdiplus::Rect(130, 10, m_pImage->GetWidth(), m_pImage->GetHeight());
	m_HpPositions[1] = Gdiplus::Rect(180, 10, m_pImage->GetWidth(), m_pImage->GetHeight());
	m_HpPositions[2] = Gdiplus::Rect(230, 10, m_pImage->GetWidth(), m_pImage->GetHeight());
	m_HpPositions[3] = Gdiplus::Rect(280, 10, m_pImage->GetWidth(), m_pImage->GetHeight());
	m_HpPositions[4] = Gdiplus::Rect(330, 10, m_pImage->GetWidth(), m_pImage->GetHeight());
		
	m_pBuffer = new Gdiplus::Bitmap(m_PanelRect.Width, m_PanelRect.Height, PixelFormat24bppRGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	DrawOnBuffer();
}

void CrystalHp::Destroy()
{
	__super::Destroy();
}

void CrystalHp::Update()
{
	int hp = GameData::Get().GetCenterCrystal()->GetHp();
	if (hp != m_LastHp)
	{
		m_LastHp = hp;

		DrawOnBuffer();
	}

	if (!m_IsDestroyed)
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

void CrystalHp::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawImage(m_pBuffer, m_PanelRect);
}

void CrystalHp::DrawOnBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);

	GDIRenderer::Get().DrawString(m_pGraphics, m_Text.c_str(), Gdiplus::Color::Black, m_TextPoint, m_TextSize);
	for (int i = 0; i < m_LastHp; ++i)
	{
		m_pGraphics->DrawImage(m_pImage, m_HpPositions[i]);
	}
}
