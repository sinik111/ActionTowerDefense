#pragma once

#include "Singleton.h"

class Object;
class Collider;
class Vector2;

class CollisionManager
	: public Singleton<CollisionManager>
{
	friend class Singleton<CollisionManager>;

private:
	std::unordered_map<std::wstring, std::vector<Object*>> m_CandidateGroups;

private:
	CollisionManager() = default;
	~CollisionManager() = default;

public:
	void ClearCandidates();
	void RegisterGameObject(const std::wstring& groupName, Object* object);

public:
	void CheckCollision(const std::wstring& group1Name, const std::wstring& group2Name);
	//void AABBCollisionPlayerAndBoxes();

public:
	static bool CheckUIRectContainPosition(const Collider& uiCollider, const Vector2& position);

private:
	bool IsAABBCollide(const Collider& aabb1, const Collider& aabb2);
	bool IsCircleCollide(const Collider& circle1, const Collider& circle2);
};