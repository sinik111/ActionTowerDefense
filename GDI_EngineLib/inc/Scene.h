#pragma once

class Object;
class Camera;
enum class ResultCode;

class Scene
{
protected:
	std::vector<Object*> m_Objects;
	std::vector<Object*> m_LateUpdateObjects;
	std::vector<Object*> m_PendingCreatedObjects;
	std::vector<Object*> m_PendingDestroyedObjects;
	Camera* m_pCamera;

public:
	Scene();
	virtual ~Scene();

	template<typename T, typename... Args>
	T* CreateObject(Args&&... args)
	{
		Object* pObject = new T(std::forward<Args>(args)...);
		m_Objects.push_back(pObject);
		pObject->Initialize();

		return (T*)pObject;
	}

	template<typename T, typename... Args>
	T* CreatePendingObject(Args&&... args)
	{
		Object* pObject = new T(std::forward<Args>(args)...);
		m_PendingCreatedObjects.push_back(pObject);
		pObject->Initialize();

		return (T*)pObject;
	}

public:
	virtual ResultCode Load() = 0;
	virtual void Enter();
	virtual void Exit();
	virtual void Unload() = 0;

public:
	virtual void Update();
	//virtual void Render();

public:
	Camera* GetCamera();
	void AddLateUpdateObject(Object* pObject);

private:
	void Clear();
};