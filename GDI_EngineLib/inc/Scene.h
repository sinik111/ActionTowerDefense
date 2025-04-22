#pragma once

class Object;
class Camera;
enum class ResultCode;

class Scene
{
protected:
	std::vector<Object*> m_Objects;
	Camera* m_pCamera;

public:
	Scene() = default;
	virtual ~Scene();

	template<typename T>
	T* CreateObject()
	{
		T* pObject = new T();
		m_Objects.push_back(pObject);

		return pObject;
	}

public:
	virtual ResultCode Load() = 0;
	virtual void Enter();
	virtual void Exit();
	virtual void Unload() = 0;

public:
	virtual void Update();
	virtual void Render();

public:
	Camera* GetCamera();

private:
	void Clear();
};