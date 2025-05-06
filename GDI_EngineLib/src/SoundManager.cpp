#include "pch.h"
#include "SoundManager.h"

#include "../lib/fmod/inc/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

SoundManager::SoundManager()
	: m_BgmChannel(nullptr)
{
	FMOD::System_Create(&m_System);

	m_System->init(128, FMOD_INIT_NORMAL, nullptr);
}

SoundManager::~SoundManager()
{
	for (auto& iter : m_Sounds)
	{
		iter.second->release();
	}

	m_System->close();
	m_System->release();
}

void SoundManager::CreateSound(const std::string fileName, const std::wstring soundName)
{
	FMOD::Sound* sound = nullptr;

	m_System->createSound(fileName.c_str(), FMOD_DEFAULT | FMOD_CREATESAMPLE, nullptr, &sound);

	m_Sounds[soundName] = sound;
}

void SoundManager::PlayBGM(const std::wstring soundName, float volume)
{
	FMOD::Sound* sound = m_Sounds[soundName];

	sound->setMode(FMOD_LOOP_NORMAL);

	m_System->playSound(sound, nullptr, false, &m_BgmChannel);

	m_BgmChannel->setVolume(volume);
}

void SoundManager::PlaySound(const std::wstring soundName, float volume)
{
	FMOD::Channel* channel = nullptr;

	m_System->playSound(m_Sounds[soundName], nullptr, false, &channel);

	channel->setVolume(volume);
}

void SoundManager::StopBGM()
{
	if (m_BgmChannel != nullptr)
	{
		m_BgmChannel->stop();
		m_BgmChannel = nullptr;
	}
}

void SoundManager::Update()
{
	m_System->update();
}
