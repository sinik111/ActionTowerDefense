#pragma once

#include "Vector2.h"

enum class ColliderType
{
	Circle, 
	AABB,
	UIRect
};

class Collider
{
public:
	Vector2 position;
	Vector2 offset;
	float width;
	float height;
	ColliderType type;

public:
	void SetColliderInfo(ColliderType type, const Vector2& position, const Vector2& offset, float width, float height);

	void UpdateCollider(const Vector2& position);
};
