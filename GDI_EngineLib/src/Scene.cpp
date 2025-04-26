#include "pch.h"
#include "Scene.h"

#include "Object.h"
#include "RenderManager.h"
#include "Camera.h"
#include "GDIRenderer.h"

Scene::~Scene()
{
	Clear();
}

void Scene::Enter()
{
	m_pCamera = CreateObject<Camera>(Vector2::Zero, GDIRenderer::Get().GetWidth(), GDIRenderer::Get().GetHeight());
}

void Scene::Exit()
{
	RenderManager::Get().ClearObjects();

	Clear();
}

void Scene::Update()
{
	if (!m_PendingCreatedObjects.empty())
	{
		m_Objects.insert(m_Objects.end(),
			std::make_move_iterator(m_PendingCreatedObjects.begin()),
			std::make_move_iterator(m_PendingCreatedObjects.end()));

		m_PendingCreatedObjects.clear();
	}

	for (size_t i = 0; i < m_Objects.size(); )
	{
		m_Objects[i]->Update();

		if (m_Objects[i]->IsDestroyed())
		{
			std::swap(m_Objects[i], m_Objects.back());

			m_PendingDestroyedObjects.push_back(m_Objects.back());

			m_Objects.pop_back();

			continue;
		}

		++i;
	}

	if (!m_PendingDestroyedObjects.empty())
	{
		for (auto& object : m_PendingDestroyedObjects)
		{
			delete object;
		}

		m_PendingDestroyedObjects.clear();
	}
}

//void Scene::Render()
//{
//	for (auto& object : m_Objects)
//	{
//		object->Render();
//	}
//}

Camera* Scene::GetCamera()
{
	return m_pCamera;
}

void Scene::Clear()
{
	for (auto& object : m_PendingCreatedObjects)
	{
		delete object;
	}

	m_PendingCreatedObjects.clear();

	for (auto& object : m_Objects)
	{
		delete object;
	}

	m_Objects.clear();

	for (auto& object : m_PendingDestroyedObjects)
	{
		delete object;
	}

	m_PendingDestroyedObjects.clear();
}
