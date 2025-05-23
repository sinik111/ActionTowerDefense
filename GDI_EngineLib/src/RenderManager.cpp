#include "pch.h"
#include "RenderManager.h"

#include <algorithm>

#include "Object.h"
#include "SceneManager.h"
#include "Camera.h"

void RenderManager::AddObject(RenderLayer layer, const Object* object)
{
	m_RenderObjects[(int)layer].push_back(object);
}

void RenderManager::SortObjects()
{
	std::sort(m_RenderObjects[(int)RenderLayer::Object].begin(),
		m_RenderObjects[(int)RenderLayer::Object].end(),
		ObjectYComparer);

	std::sort(m_RenderObjects[(int)RenderLayer::WorldUI].begin(),
		m_RenderObjects[(int)RenderLayer::WorldUI].end(),
		ObjectYComparer);
}

void RenderManager::RenderObjects() const
{
	Camera* pCamera = SceneManager::Get().GetCurrentCamera();

	for (int i = 0; i < (int)RenderLayer::MAX; ++i)
	{
		for (auto object : m_RenderObjects[i])
		{
			object->Render(*pCamera);
		}
	}
}

void RenderManager::ClearObjects()
{
	for (int i = 0; i < (int)RenderLayer::MAX; ++i)
	{
		m_RenderObjects[i].clear();
	}
}

bool RenderManager::ObjectYComparer(const Object* a, const Object* b)
{
	return a->GetPosition().y < b->GetPosition().y;
}
