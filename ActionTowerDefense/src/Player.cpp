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
#include "GameData.h"

Player::Player()
	: m_Speed(0.0f), m_current_state(PlayerAnimState::IdleDown), m_next_state(PlayerAnimState::IdleDown),
	m_AttackDirection{}, m_pAttackImage{}, m_ColliderDistance(0.0f), m_ImageDistance(0.0f),
	m_AttackDuration(0.2f), m_IsStartAttack(false), m_IsCollide(false), m_AttackDurationTimer(0.0f),
	m_CurrentDirection(AttackPositionType::Max)
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

	while (wss >> animName >> groupName >> frames >> duration >> isLoop)
	{
		std::vector<Gdiplus::Bitmap*> images;

		for (int i = 0; i < frames; ++i)
		{
			wss >> resourceName;

			images.push_back(ResourceManager::Get().GetImage(groupName, resourceName));
		}

		m_animation_controller.AddAnimation(animName, new Animation(images, frames, duration, isLoop));
	}

	m_animation_controller.PlayAnimation(L"IdleDown");

	m_Position = Vector2(128 * 29 / 2, 128 * 29 / 2);

	m_Speed = 200.0f;

	m_Collider = Collider(m_Position, 300.0f);

	m_pAttackImage[(int)AttackPositionType::Up] = ResourceManager::Get().GetImage(L"Play", L"AttackUp");
	m_pAttackImage[(int)AttackPositionType::Down] = ResourceManager::Get().GetImage(L"Play", L"AttackDown");
	m_pAttackImage[(int)AttackPositionType::Left] = ResourceManager::Get().GetImage(L"Play", L"AttackLeft");
	m_pAttackImage[(int)AttackPositionType::Right] = ResourceManager::Get().GetImage(L"Play", L"AttackRight");
	m_pAttackImage[(int)AttackPositionType::LeftUp] = ResourceManager::Get().GetImage(L"Play", L"AttackLeftUp");
	m_pAttackImage[(int)AttackPositionType::LeftDown] = ResourceManager::Get().GetImage(L"Play", L"AttackLeftDown");
	m_pAttackImage[(int)AttackPositionType::RightUp] = ResourceManager::Get().GetImage(L"Play", L"AttackRightUp");
	m_pAttackImage[(int)AttackPositionType::RightDown] = ResourceManager::Get().GetImage(L"Play", L"AttackRightDown");

	m_AttackCollider = Collider(Vector2::Zero, 100.0f);
	m_ColliderDistance = 100.0f;
	m_ImageDistance = 150.0f;

	m_AttackDirection[(int)AttackPositionType::Up] = Vector2::Up;
	m_AttackDirection[(int)AttackPositionType::Down] = Vector2::Down;
	m_AttackDirection[(int)AttackPositionType::Left] = Vector2::Left;
	m_AttackDirection[(int)AttackPositionType::Right] = Vector2::Right;
	m_AttackDirection[(int)AttackPositionType::LeftUp] = (Vector2::Left + Vector2::Up).Normalized();
	m_AttackDirection[(int)AttackPositionType::LeftDown] = (Vector2::Left + Vector2::Down).Normalized();
	m_AttackDirection[(int)AttackPositionType::RightUp] = (Vector2::Right + Vector2::Up).Normalized();
	m_AttackDirection[(int)AttackPositionType::RightDown] = (Vector2::Right + Vector2::Down).Normalized();

	GameData::Get().SetPlayer(this);
}

void Player::Destroy()
{
	__super::Destroy();
}

void Player::Update()
{
	m_animation_controller.Update();

	Vector2 direction = Vector2::Zero;

	if (Input::IsKeyDown('W') || Input::IsKeyDown('A')
		|| Input::IsKeyDown('S') || Input::IsKeyDown('D'))
	{
		if (Input::IsKeyDown('D'))
		{
			direction += Vector2::Right;
			m_next_state = PlayerAnimState::WalkRight;
		}

		if (Input::IsKeyDown('A'))
		{
			direction += Vector2::Left;
			m_next_state = PlayerAnimState::WalkLeft;
		}

		if (Input::IsKeyDown('W'))
		{
			direction += Vector2::Up;
			m_next_state = PlayerAnimState::WalkUp;
		}

		if (Input::IsKeyDown('S'))
		{
			direction += Vector2::Down;
			m_next_state = PlayerAnimState::WalkDown;
		}

		if (direction.IsZero())
		{
			switch (m_current_state)
			{
			case PlayerAnimState::WalkRight:
				m_next_state = PlayerAnimState::IdleRight;
				break;
			case PlayerAnimState::WalkLeft:
				m_next_state = PlayerAnimState::IdleLeft;
				break;
			case PlayerAnimState::WalkUp:
				m_next_state = PlayerAnimState::IdleUp;
				break;
			case PlayerAnimState::WalkDown:
				m_next_state = PlayerAnimState::IdleDown;
				break;
			}
		}
	}
	else
	{
		switch (m_current_state)
		{
		case PlayerAnimState::WalkRight:
			m_next_state = PlayerAnimState::IdleRight;
			break;
		case PlayerAnimState::WalkLeft:
			m_next_state = PlayerAnimState::IdleLeft;
			break;
		case PlayerAnimState::WalkUp:
			m_next_state = PlayerAnimState::IdleUp;
			break;
		case PlayerAnimState::WalkDown:
			m_next_state = PlayerAnimState::IdleDown;
			break;
		}
	}


	if (m_current_state != m_next_state)
	{
		switch (m_next_state)
		{
		case PlayerAnimState::WalkRight:
			m_current_state = PlayerAnimState::WalkRight;
			m_animation_controller.PlayAnimation(L"WalkRight");
			break;
		case PlayerAnimState::WalkLeft:
			m_current_state = PlayerAnimState::WalkLeft;
			m_animation_controller.PlayAnimation(L"WalkLeft");
			break;
		case PlayerAnimState::WalkUp:
			m_current_state = PlayerAnimState::WalkUp;
			m_animation_controller.PlayAnimation(L"WalkUp");
			break;
		case PlayerAnimState::WalkDown:
			m_current_state = PlayerAnimState::WalkDown;
			m_animation_controller.PlayAnimation(L"WalkDown");
			break;
		case PlayerAnimState::IdleRight:
			m_current_state = PlayerAnimState::IdleRight;
			m_animation_controller.PlayAnimation(L"IdleRight");
			break;
		case PlayerAnimState::IdleLeft:
			m_current_state = PlayerAnimState::IdleLeft;
			m_animation_controller.PlayAnimation(L"IdleLeft");
			break;
		case PlayerAnimState::IdleUp:
			m_current_state = PlayerAnimState::IdleUp;
			m_animation_controller.PlayAnimation(L"IdleUp");
			break;
		case PlayerAnimState::IdleDown:
			m_current_state = PlayerAnimState::IdleDown;
			m_animation_controller.PlayAnimation(L"IdleDown");
			break;
		}
	}

	if (!direction.IsZero())
	{
		m_Position += direction.Normalized() * m_Speed * MyTime::DeltaTime();
	}

	SceneManager::Get().GetCurrentCamera()->SetPosition(m_Position);

	m_Collider.UpdateCollider(m_Position);

	if (!m_IsStartAttack)
	{
		if (Input::IsKeyReleased(VK_RBUTTON))
		{
			m_IsStartAttack = true;

			Vector2 mousePos = Input::GetCursorPosition();

			int width = GDIRenderer::Get().GetWidth();
			int height = GDIRenderer::Get().GetHeight();

			Vector2 centerPos = Vector2(width / 2.0f, height / 2.0f);

			Vector2 mouseDir = Vector2::Direction(mousePos, centerPos);

			float rightDot = Vector2::Dot(Vector2::Right, mouseDir);
			float upDot = Vector2::Dot(Vector2::Up, mouseDir);

			if (rightDot > 0.9f)
			{
				m_CurrentDirection = AttackPositionType::Right;
			}
			else if (rightDot < -0.9f)
			{
				m_CurrentDirection = AttackPositionType::Left;
			}
			else if (rightDot >= 0.1f && rightDot <= 0.9f)
			{
				if (upDot > 0)
				{
					m_CurrentDirection = AttackPositionType::RightUp;
				}
				else
				{
					m_CurrentDirection = AttackPositionType::RightDown;
				}
			}
			else if (upDot > 0.9f)
			{
				m_CurrentDirection = AttackPositionType::Up;
			}
			else if (upDot < -0.9f)
			{
				m_CurrentDirection = AttackPositionType::Down;
			}
			else if (rightDot >= -0.9f && rightDot <= -0.1f)
			{
				if (upDot > 0)
				{
					m_CurrentDirection = AttackPositionType::LeftUp;
				}
				else
				{
					m_CurrentDirection = AttackPositionType::LeftDown;
				}
			}

			// x > 0.75
			// 0.25 < x < 0.75
			// -0.25 < x < 0.25
			// -0.75 < x < -.025
			// x < 0.75

			// y < 0
			// y > 0
		}
	}

	if (m_IsStartAttack)
	{
		m_AttackDurationTimer += MyTime::DeltaTime();
		if (!m_IsCollide)
		{
			m_IsCollide = true;
			CollisionManager::Get().RegisterGameObject(L"PlayerRange", this);
		}

		if (m_AttackDurationTimer > m_AttackDuration)
		{
			m_IsStartAttack = false;
			m_IsCollide = false;
			m_AttackDurationTimer = 0.0f;
		}
	}

	if (m_IsCollide && !m_pInRangeObjects.empty())
	{
		Vector2 attackPosition = m_Position + m_AttackDirection[(int)m_CurrentDirection] * m_ColliderDistance;
		m_AttackCollider.UpdateCollider(attackPosition);

		for (auto& object : m_pInRangeObjects)
		{
			if (CollisionManager::Get().IsCircleCollide(object->GetCollider(), m_AttackCollider))
			{
				object->Collide(this, L"PlayerAttack");	
			}
		}

		m_pInRangeObjects.clear();
	}

	if (!m_IsDestroyed)
	{
		//CollisionManager::Get().RegisterGameObject(L"Player", this);
		GameData::Get().RegisterMiniMapInfo(m_Position, MiniMapObjectType::Player);
		//Debug::Log(m_Position.ToString());
	}

	__super::Update();
}

void Player::Render(const Camera& camera) const
{
	Gdiplus::Bitmap* image;
	Gdiplus::Rect dstRect;
	Gdiplus::Rect srcRect;

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	m_animation_controller.GetCurrentAnimation()->GetFrameInfo(image, dstRect, srcRect);

	dstRect.X = (int)(cameraViewPos.x - srcRect.Width / 2);
	dstRect.Y = (int)(cameraViewPos.y - srcRect.Height / 2);

	GDIRenderer::Get().DrawImage(image, dstRect, srcRect);

	if (m_IsStartAttack)
	{
		Gdiplus::Rect dstRect2;
		dstRect2.Width = m_pAttackImage[(int)m_CurrentDirection]->GetWidth();
		dstRect2.Height = m_pAttackImage[(int)m_CurrentDirection]->GetHeight();

		Vector2 imagePosition = m_Position + m_AttackDirection[(int)m_CurrentDirection] * m_ImageDistance;

		Vector2 cameraViewImagePos = camera.ToCameraView(imagePosition);

		dstRect2.X = (int)(cameraViewImagePos.x - dstRect2.Width / 2);
		dstRect2.Y = (int)(cameraViewImagePos.y - dstRect2.Height / 2);

		GDIRenderer::Get().DrawImage(m_pAttackImage[(int)m_CurrentDirection], dstRect2);
	}
}

void Player::Collide(Object* object, const std::wstring& groupName)
{
	if (groupName == L"Enemy")
	{
		m_pInRangeObjects.push_back(object);
	}
}
