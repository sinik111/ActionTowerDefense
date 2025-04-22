#include "pch.h"
#include "CollisionManager.h"

#include "Object.h"
#include "Collider.h"
#include "Vector2.h"

void CollisionManager::ClearCandidates()
{
	m_CandidateGroups.clear();
}

void CollisionManager::ResisterGameObject(const std::wstring& groupName, Object* object)
{
	m_CandidateGroups[groupName].push_back(object);
}

void CollisionManager::CheckCollision(const std::wstring& group1Name, const std::wstring& group2Name)
{
	std::vector<Object*>& group1 = m_CandidateGroups[group1Name];
	std::vector<Object*>& group2 = m_CandidateGroups[group2Name];

	for (auto object1 : group1)
	{
		for (auto object2 : group2)
		{
			switch (object1->GetCollider().type)
			{
			case ColliderType::AABB:
				if (IsAABBCollide(object1->GetCollider(), object2->GetCollider()))
				{
					object1->Collide(*object2, group2Name);
					object2->Collide(*object1, group1Name);
				}
				break;
			}
			
		}
	}
}

//void CollisionManager::AABBCollisionPlayerAndBoxes()
//{
//	auto player_iter = m_CandidateGroups[(int)CandidateType::Player].Begin();
//	auto boxes_iter = m_CandidateGroups[(int)CandidateType::Box].Begin();
//	while (!player_iter.IsEnd())
//	{
//		AABB player_collider = (*player_iter)->GetCollider().GetAABBCollider();
//
//		while (!boxes_iter.IsEnd())
//		{
//			AABB box_collider = (*boxes_iter)->GetCollider().GetAABBCollider();
//
//			if (IsAABBCollide(player_collider, box_collider))
//			{
//				(*boxes_iter)->Destroy();
//			}
//
//			++boxes_iter;
//		}
//
//		++player_iter;
//	}
//}

bool CollisionManager::CheckUIRectContainPosition(const Collider& uiRect, const Vector2& position)
{
	return !(uiRect.position.x > position.x || uiRect.position.y > position.y ||
		uiRect.position.x + uiRect.width < position.x ||
		uiRect.position.y + uiRect.height < position.y);
}

bool CollisionManager::IsAABBCollide(const Collider& aabb1, const Collider& aabb2)
{
	return !(aabb1.position.x > aabb2.position.x + aabb2.width ||
		aabb1.position.x + aabb1.width < aabb2.position.x ||
		aabb1.position.y > aabb2.position.y + aabb2.height ||
		aabb1.position.y + aabb1.height < aabb2.position.y);
}

bool CollisionManager::IsCircleCollide(const Collider& circle1, const Collider& circle2)
{
	return Vector2::SquareDistance(circle1.position, circle2.position) <=
		(circle1.width + circle2.width) * (circle1.width + circle2.width);
}
