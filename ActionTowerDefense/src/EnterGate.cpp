#include "framework.h"
#include "EnterGate.h"

#include "RenderManager.h"
#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "Camera.h"
#include "GameData.h"
#include "SceneManager.h"

EnterGate::EnterGate(int gateNumber, int row, int column)
	: m_pImage(nullptr), m_GateNumber(gateNumber)
{
	m_Position = Vector2(128.0f * row + 64.0f, 128.0f * column + 64.0f);

	m_RenderLayer = RenderLayer::Object;
}

void EnterGate::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"EnterGate");
}

void EnterGate::Destroy()
{
	__super::Destroy();
}

void EnterGate::Update()
{
	if (!m_IsDestroyed)
	{
		GameData::Get().RegisterMiniMapInfo(m_Position, MiniMapObjectType::EnterGate);
		if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position, m_pImage->GetWidth(), m_pImage->GetHeight()))
		{
			RenderManager::Get().AddObject(m_RenderLayer, this);
		}
	}

	//__super::Update();
}

void EnterGate::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect, srcRect;
	srcRect.X = 0;
	srcRect.Y = 0;
	dstRect.Width = srcRect.Width = m_pImage->GetWidth();
	dstRect.Height = srcRect.Height = m_pImage->GetHeight();

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	dstRect.X = (int)(cameraViewPos.x - srcRect.Width / 2);
	dstRect.Y = (int)(cameraViewPos.y - srcRect.Height / 2);

	GDIRenderer::Get().DrawImage(m_pImage, dstRect, srcRect);

	//Debug::Log("Draw");
}
