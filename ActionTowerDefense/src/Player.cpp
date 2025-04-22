#include "framework.h"
#include "Player.h"

#include <sstream>

#include "ResultCode.h"
#include "AnimationController.h"
#include "Animation.h"
#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "Input.h"
#include "MyTime.h"
#include "Camera.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "SceneManager.h"

Player::Player()
	: m_Speed(0.0f), m_current_state(PlayerState::None), m_next_state(PlayerState::None),
	m_Score(0)
{
	m_RenderLayer = RenderLayer::Object;
}

void Player::Initialize()
{
	Gdiplus::Bitmap* image = nullptr;
	Animation* pAnimation = nullptr;

	std::wstring data = ResourceManager::Get().GetString(L"Play", L"PlayerAnimationData");

	std::wstringstream wss(data);

	std::wstring animName, groupName, resourceName;
	int frames;
	float duration;
	bool isLoop;

	while (!wss.eof())
	{
		wss >> animName >> groupName >> resourceName >> frames >> duration >> isLoop;

		image = ResourceManager::Get().GetImage(groupName, resourceName);
		m_animation_controller.AddAnimation(animName, new Animation(image, frames, duration, isLoop));
	}

	m_animation_controller.PlayAnimation(L"Idle");

	m_current_state = PlayerState::Idle;
	m_next_state = PlayerState::Idle;

	m_Position = Vector2(128 * 29 / 2, 128 * 29 / 2);

	m_Speed = 200.0f;

	m_Collider.SetColliderInfo(ColliderType::AABB, m_Position, Vector2(0.0f, 27.0f), 35.0f, 75.0f);
}

void Player::Destroy()
{
	__super::Destroy();
}

void Player::Update()
{
	m_animation_controller.Update();

	Vector2 direction = Vector2::Zero;

	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT)
		|| Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_DOWN))
	{
		if (Input::IsKeyDown(VK_RIGHT))
		{
			direction += Vector2::Right;
		}

		if (Input::IsKeyDown(VK_LEFT))
		{
			direction += Vector2::Left;
		}

		if (Input::IsKeyDown(VK_UP))
		{
			direction += Vector2::Up;
		}

		if (Input::IsKeyDown(VK_DOWN))
		{
			direction += Vector2::Down;
		}

		m_next_state = PlayerState::Walk;
	}
	else
	{
		m_next_state = PlayerState::Idle;
	}

	if (Input::IsKeyReleased('A'))
	{
		m_next_state = PlayerState::Attack;
	}

	if (m_current_state != m_next_state)
	{
		switch (m_next_state)
		{
		case PlayerState::Attack:
			if (m_current_state == PlayerState::Idle || m_current_state == PlayerState::Walk)
			{
				m_animation_controller.PlayAnimation(L"Attack");

				m_current_state = m_next_state;
			}
			break;

		case PlayerState::Idle:
			if (m_current_state == PlayerState::Walk)
			{
				m_animation_controller.PlayAnimation(L"Idle");

				m_current_state = m_next_state;
			}
			else if (m_animation_controller.GetCurrentAnimation()->IsEnd())
			{
				m_animation_controller.PlayAnimation(L"Idle");

				m_current_state = m_next_state;
			}
			break;

		case PlayerState::Walk:
			if (m_current_state == PlayerState::Idle)
			{
				m_animation_controller.PlayAnimation(L"Walk");

				m_current_state = m_next_state;
			}
			else if (m_animation_controller.GetCurrentAnimation()->IsEnd())
			{
				m_animation_controller.PlayAnimation(L"Walk");

				m_current_state = m_next_state;
			}
			break;
		}
	}

	if (Input::IsKeyReleased('D'))
	{
		//m_
	}

	if (m_current_state == PlayerState::Walk)
	{
		if (!direction.IsZero())
			m_Position += direction.Normalized() * m_Speed * MyTime::DeltaTime();
	}

	SceneManager::Get().GetCurrentCamera()->SetPosition(m_Position);

	m_Collider.UpdateCollider(m_Position);

	if (!m_IsDestroyed)
	{
		CollisionManager::Get().RegisterGameObject(L"Player", this);
	}

	__super::Update();
}

void Player::Render(const Camera& camera) const
{
	Gdiplus::Bitmap* image;
	Gdiplus::Rect dst_rect;
	Gdiplus::Rect src_rect;

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	m_animation_controller.GetCurrentAnimation()->GetFrameInfo(image, dst_rect, src_rect);

	dst_rect.X = (int)(cameraViewPos.x - src_rect.Width / 2);
	dst_rect.Y = (int)(cameraViewPos.y - src_rect.Height / 2);

	GDIRenderer::Get().DrawImage(image, dst_rect, src_rect);

#ifdef _DEBUG
	Vector2 collierCameraViewPos = camera.ToCameraView(m_Collider.position);

	GDIRenderer::Get().DrawRectangle(Gdiplus::Color(0, 0, 0), Gdiplus::Rect((int)collierCameraViewPos.x, (int)collierCameraViewPos.y,
		(int)m_Collider.width, (int)m_Collider.height));
#endif // _DEBUG
}

void Player::Collide(Object& object, const std::wstring& groupName)
{
	//if (groupName == L"box")
	//{
	//	++m_Score;
	//	GameData::Get().SetScore(m_Score);
	//}
}
