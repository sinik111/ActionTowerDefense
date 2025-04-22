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
	m_pCamera = new Camera(Vector2::Zero, GDIRenderer::Get().GetWidth(), GDIRenderer::Get().GetHeight());

	m_Objects.push_back(m_pCamera);
}

void Scene::Exit()
{
	RenderManager::Get().ClearObjects();

	Clear();
}

void Scene::Update()
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		(*iter)->Update();

		if ((*iter)->IsDestroyed())
		{
			delete (*iter);

			iter = m_Objects.erase(iter);

			continue;
		}

		++iter;
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
	for (auto& object : m_Objects)
	{
		delete object;
	}

	m_Objects.clear();
}
