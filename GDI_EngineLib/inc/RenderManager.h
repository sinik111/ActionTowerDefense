#pragma once

#include "Singleton.h"

enum class RenderLayer
{
	Background,
	Object,
	WorldUI,
	ScreenUI,
	MAX
};

class Object;

class RenderManager :
	public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
private:
	std::vector<const Object*> m_RenderObjects[(int)RenderLayer::MAX];

private:
	RenderManager() = default;
	~RenderManager() = default;

public:
	void AddObject(RenderLayer layer, const Object* object);
	void SortObjects();
	void RenderObjects() const;
	void ClearObjects();

private:
	static bool ObjectYComparer(const Object* a, const Object* b);
};