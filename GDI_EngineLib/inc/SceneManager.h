#pragma once

#include "Singleton.h"
#include "ResultCode.h"
#include "Debug.h"
#include "Camera.h"
#include "Scene.h"

class SceneManager
	: public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

private:
	std::unordered_map<std::wstring, Scene*> m_Scenes;

	Scene* m_pCurrentScene;
	Scene* m_pNextScene;

private:
	SceneManager();
	~SceneManager() = default;

public:
	void Shutdown();

public:
	void Update();
	//void Render();

public:
	void ChangeScene(const std::wstring& name);
	Scene* GetCurrentScene();
	Camera* GetCurrentCamera();

private:
	void CheckSceneChanged();

public:
	template<typename T>
	T* CreateScene(const std::wstring& name)
	{
		if (m_Scenes.find(name) != m_Scenes.end())
		{
			Debug::Log("Has same name - SceneManager::CreateScene");

			return nullptr;
		}

		T* pScene = new T();
		m_Scenes.emplace(name, pScene);

		return pScene;
	}
};