#pragma once

class Animation;

class AnimationController
{
private:
	std::unordered_map<std::wstring, Animation*> m_Animations;
	Animation* m_pCurrentAnimation;

public:
	AnimationController();
	~AnimationController();

public:
	void AddAnimation(const std::wstring& name, Animation* animation);
	void PlayAnimation(const std::wstring& name);
	void Update();
	Animation* GetCurrentAnimation() const;
};