#pragma once

#include "Singleton.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class SoundManager :
	public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;

private:
	FMOD::System* m_System;
	std::unordered_map<std::wstring, FMOD::Sound*> m_Sounds;
	FMOD::Channel* m_BgmChannel;

private:
	SoundManager();
	~SoundManager();

public:
	void CreateSound(const std::string fileName, const std::wstring soundName);
	void PlayBGM(const std::wstring soundName, float volume);
	void PlaySound(const std::wstring soundName, float volume);
	void StopBGM();

	void Update();
};