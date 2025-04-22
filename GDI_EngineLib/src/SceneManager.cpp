#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"
#include "Debug.h"

SceneManager::SceneManager()
	: m_pCurrentScene(nullptr), m_pNextScene(nullptr)
{
}

void SceneManager::Shutdown()
{
	for (auto& pair : m_Scenes)
	{
		delete pair.second;
	}

	m_Scenes.clear();
}

void SceneManager::Update()
{
	CheckSceneChanged();

	m_pCurrentScene->Update();
}

//void SceneManager::Render()
//{
//	m_pCurrentScene->Render();
//}

void SceneManager::ChangeScene(const std::wstring& name)
{
	if (m_Scenes.find(name) == m_Scenes.end())
	{
		Debug::Log("Wrong name - SceneManager::CreateScene");

		return;
	}

	m_pNextScene = m_Scenes[name];
}

Scene* SceneManager::GetCurrentScene()
{
	return m_pCurrentScene;
}

Camera* SceneManager::GetCurrentCamera()
{
	return m_pCurrentScene->GetCamera();
}

void SceneManager::CheckSceneChanged()
{
	if (m_pNextScene != nullptr)
	{
		if (ResultCode::OK != m_pNextScene->Load())
		{
			m_pNextScene->Unload();

			Debug::Log("Scene load failed - SceneManager::CheckSceneChanged");

			m_pNextScene = nullptr;

			return;
		}

		if (m_pCurrentScene != nullptr)
		{
			m_pCurrentScene->Exit();
			m_pCurrentScene->Unload();
		}

		m_pCurrentScene = m_pNextScene;
		m_pNextScene = nullptr;

		m_pCurrentScene->Enter();
	}
}
