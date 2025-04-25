#include "framework.h"
#include "TowerButton.h"

#include "TowerPlace.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "Constant.h"
#include "CollisionManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "TowerBuildButton.h"

TowerButton::TowerButton(TowerPlace* pTargetTowerPlace)
	: m_pTargetTowerPlace(pTargetTowerPlace), m_CurrentImageIndex(0), m_pImages{},
	m_IsButtonDown(false), m_IsBuildButtonCreated(false)
{
	m_RenderLayer = RenderLayer::WorldUI;
	m_SrcRect = Gdiplus::Rect(0, 0, TOWER_BUTTON_SIZE, TOWER_BUTTON_SIZE);
	m_Position = pTargetTowerPlace->GetPosition() +
		Vector2(TILE_SIZE / 2 - TOWER_BUTTON_SIZE, TILE_SIZE / 2 - TOWER_BUTTON_SIZE);
}

void TowerButton::Initialize()
{
	m_Collider.SetColliderInfo(ColliderType::UIRect, m_Position, Vector2::Zero, TOWER_BUTTON_SIZE, TOWER_BUTTON_SIZE);

	m_pImages[0] = ResourceManager::Get().GetImage(L"Play", L"TowerButton1");
	m_pImages[1] = ResourceManager::Get().GetImage(L"Play", L"TowerButton2");
}

void TowerButton::Destroy()
{
	__super::Destroy();
}

void TowerButton::Update()
{
	m_Collider.UpdateCollider(m_Position);

	const Camera* camera = SceneManager::Get().GetCurrentCamera();

	if (!camera->IsUIOutOfView(m_Position, m_SrcRect.Width, m_SrcRect.Height))
	{
		if (CollisionManager::Get().CheckUIRectContainPosition(m_Collider, camera->ToWorldView(Input::GetCursorPosition())))
		{
			if (Input::IsKeyPressed(VK_LBUTTON))
			{
				m_IsButtonDown = true;
			}

			if (Input::IsKeyReleased(VK_LBUTTON) && m_IsButtonDown)
			{
				SceneManager::Get().GetCurrentScene()->CreatePendingObject<TowerBuildButton>(m_pTargetTowerPlace);

				m_IsBuildButtonCreated = true;
				m_IsButtonDown = false;
			}

			if (Input::IsKeyDown(VK_LBUTTON) && m_IsButtonDown)
			{
				m_CurrentImageIndex = 1;
			}
			else
			{
				m_CurrentImageIndex = 0;
				m_IsButtonDown = false;
			}
		}
		else
		{
			m_CurrentImageIndex = 0;
			m_IsButtonDown = false;
		}

		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
	else
	{
		m_CurrentImageIndex = 0;
		m_IsButtonDown = false;
	}
}

void TowerButton::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect;

	Vector2 cameraViewPosition = camera.ToCameraView(m_Position);

	dstRect.X = (int)cameraViewPosition.x;
	dstRect.Y = (int)cameraViewPosition.y;
	dstRect.Width = m_SrcRect.Width;
	dstRect.Height = m_SrcRect.Height;

	GDIRenderer::Get().DrawImage(m_pImages[m_CurrentImageIndex], dstRect, m_SrcRect);
}
