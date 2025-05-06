#include "framework.h"
#include "TimerUI.h"

#include "GDIRenderer.h"
#include "GameData.h"

TimerUI::TimerUI()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_TextSize(30),
	m_CurrentTime(0)
{
	m_RenderLayer = RenderLayer::ScreenUI;
}

TimerUI::~TimerUI()
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

void TimerUI::Initialize()
{
	m_CurrentTime = GameData::Get().GetRemainReadyTime();

	m_Text = L"0:" + std::to_wstring(m_CurrentTime);

	m_TextPoint = Gdiplus::PointF(40.0f, 10.0f);

	m_PanelRect = Gdiplus::Rect(565, 0, 150, 70);

	m_pBuffer = new Gdiplus::Bitmap(m_PanelRect.Width, m_PanelRect.Height, PixelFormat24bppRGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	DrawOnBuffer();
}

void TimerUI::Destroy()
{
	__super::Destroy();
}

void TimerUI::Update()
{
	switch (GameData::Get().GetCurrentGameState())
	{
	case GameState::Ready:
		if (m_CurrentTime != GameData::Get().GetRemainReadyTime())
		{
			m_CurrentTime = GameData::Get().GetRemainReadyTime();
			if (m_CurrentTime < 10)
			{
				m_Text = L"0:0" + std::to_wstring(m_CurrentTime);
			}
			else
			{
				m_Text = L"0:" + std::to_wstring(m_CurrentTime);
			}

			DrawOnBuffer();
		}
		break;

	case GameState::Play:
		if (m_CurrentTime != GameData::Get().GetRemainPlayTime())
		{
			m_CurrentTime = GameData::Get().GetRemainPlayTime();

			int minute = m_CurrentTime / 60;
			int second = m_CurrentTime % 60;

			if (minute > 0)
			{
				m_Text = std::to_wstring(minute) + L":";
			}
			else
			{
				m_Text = L"0:";
			}

			if (second < 10)
			{
				m_Text += L"0" + std::to_wstring(second);
			}
			else
			{
				m_Text += std::to_wstring(second);
			}

			DrawOnBuffer();
		}
		break;

	default:
		if (m_CurrentTime != 0)
		{
			m_CurrentTime = 0;
			m_Text = L"0:00";
			DrawOnBuffer();
		}
		break;
	}

	if (!m_IsDestroyed)
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

void TimerUI::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawImage(m_pBuffer, m_PanelRect);
}

void TimerUI::DrawOnBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);

	GDIRenderer::Get().DrawString(m_pGraphics, m_Text.c_str(), Gdiplus::Color::White, m_TextPoint, m_TextSize);
}
