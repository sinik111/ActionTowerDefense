#include "framework.h"
#include "EnemyInfoDisplay.h"

#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "GameData.h"
#include "Enemy.h"
#include "SceneManager.h"

EnemyInfoDisplay::EnemyInfoDisplay()
	: m_pBuffer(nullptr), m_pGraphics(nullptr), m_pImages{}, m_NameTextSize(18),
	m_InfoTextSize(14), m_HasEnemyInfo(false)
{
	m_RenderLayer = RenderLayer::ScreenUI;
}

EnemyInfoDisplay::~EnemyInfoDisplay()
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

void EnemyInfoDisplay::Initialize()
{
	m_pImages[(int)EnemyType::Circle] = ResourceManager::Get().GetImage(L"Play", L"EnemyCircle");
	m_pImages[(int)EnemyType::Rectangle] = ResourceManager::Get().GetImage(L"Play", L"EnemyRectangle");
	m_pImages[(int)EnemyType::Triangle] = ResourceManager::Get().GetImage(L"Play", L"EnemyTriangle");

	m_PanelRect = Gdiplus::Rect(0, 472, 248, 248);
	
	m_NameTextPoint = Gdiplus::PointF(87.0f, 10.0f);
	m_InfoTextPoint = Gdiplus::PointF(5.0f, 150.0f);
	m_ImageDstRect = Gdiplus::Rect(90, 60, m_pImages[0]->GetWidth(), m_pImages[0]->GetHeight());

	m_HpStartPoint = Gdiplus::PointF(100.0f, 160.0f);
	m_HpEndPoint = Gdiplus::PointF(100.0f, 160.0f);

	m_pBuffer = new Gdiplus::Bitmap(m_PanelRect.Width, m_PanelRect.Height, PixelFormat24bppRGB);

	m_pGraphics = new Gdiplus::Graphics(m_pBuffer);

	ClearBuffer();
}

void EnemyInfoDisplay::Destroy()
{
	__super::Destroy();
}

void EnemyInfoDisplay::Update()
{
	if (!m_IsDestroyed)
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);

		SceneManager::Get().GetCurrentScene()->AddLateUpdateObject(this);
	}
}

void EnemyInfoDisplay::LateUpdate()
{
	if (!GameData::Get().HasEnemyInfo())
	{
		if (m_HasEnemyInfo)
		{
			ClearBuffer();

			m_HasEnemyInfo = false;
		}
	}
	else
	{
		EnemyInfo info = GameData::Get().GetEnemyInfo();
		if (!(info == m_EnemyInfo))
		{
			m_EnemyInfo = info;
			switch ((EnemyType)info.type)
			{
			case EnemyType::Circle:
				m_TypeName = L"동그라미";
				break;
			case EnemyType::Rectangle:
				m_TypeName = L"  네모  ";
				break;
			case EnemyType::Triangle:
				m_TypeName = L"  세모  ";
				break;
			}

			m_InfoText = L"HP: " + std::to_wstring((int)info.hp) + L"/" +
				std::to_wstring((int)info.maxHp) + L"\n";
			if (!info.isSlowed)
			{
				m_InfoText += L"Speed: " + std::to_wstring((int)info.speed) + L"\n";
			}
			else
			{
				m_InfoText += L"Speed: " + std::to_wstring((int)(info.speed * info.slowRate)) +
					L"(-" + std::to_wstring((int)(100.0f - info.slowRate * 100.0f)) + L"%)\n";
			}

			m_InfoText += L"Debuff: ";
			if (info.isSlowed)
			{
				m_InfoText += L"Slowed(-" + std::to_wstring((int)(100.0f - info.slowRate * 100.0f)) + L"%)\n          ";
			}

			if (info.isShocked)
			{
				m_InfoText += L"Shocked(+" + std::to_wstring((int)(info.multiplier * 100.0f)) + L"%)";
			}

			m_InfoText += L"\n";
		}

		m_HpEndPoint.X = m_HpStartPoint.X + 100.0f * (info.hp / info.maxHp);

		m_HasEnemyInfo = true;

		DrawOnBuffer();
	}
}

void EnemyInfoDisplay::Render(const Camera& camera) const
{
	GDIRenderer::Get().DrawImage(m_pBuffer, m_PanelRect);
}

void EnemyInfoDisplay::DrawOnBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);

	GDIRenderer::Get().DrawString(m_pGraphics, m_TypeName.c_str(), Gdiplus::Color::Black, m_NameTextPoint, m_NameTextSize);
	m_pGraphics->DrawImage(m_pImages[m_EnemyInfo.type], m_ImageDstRect);
	GDIRenderer::Get().DrawString(m_pGraphics, m_InfoText.c_str(), Gdiplus::Color::Black, m_InfoTextPoint, m_InfoTextSize);
	GDIRenderer::Get().DrawLine(m_pGraphics, Gdiplus::Color(255, 0, 0), 4.0f, m_HpStartPoint, m_HpEndPoint);
}

void EnemyInfoDisplay::ClearBuffer()
{
	m_pGraphics->Clear(Gdiplus::Color::LightSteelBlue);
}
