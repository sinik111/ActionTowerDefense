#include "pch.h"
#include "AnimationController.h"

#include "Animation.h"
#include "Debug.h"

AnimationController::AnimationController()
	: m_pCurrentAnimation(nullptr)
{
}

AnimationController::~AnimationController()
{
	for (auto pair : m_Animations)
	{
		delete pair.second;
	}

	m_Animations.clear();
}

void AnimationController::AddAnimation(const std::wstring& name, Animation* animation)
{
	if (m_Animations.find(name) != m_Animations.end())
	{
		Debug::Log(L"Already has same name. name: " + name + L" AnimationController::AddAnimation");

		return;
	}

	m_Animations.emplace(name, animation);
}

void AnimationController::PlayAnimation(const std::wstring& name)
{
	if (m_Animations.find(name) == m_Animations.end())
	{
		Debug::Log(L"No animation. name: " + name + L" AnimationController::PlayAnimation");

		return;
	}

	if (m_pCurrentAnimation != nullptr)
	{
		m_pCurrentAnimation->Reset();
	}

	Debug::Log(L"Play animation. name: " + name + L" AnimationController::PlayAnimation");

	m_pCurrentAnimation = m_Animations[name];

	m_pCurrentAnimation->Play();
}

void AnimationController::Update()
{
	m_pCurrentAnimation->Update();
}

Animation* AnimationController::GetCurrentAnimation() const
{
	return m_pCurrentAnimation;
}
