#pragma once

#include "Vector2.h"

enum class ColliderType
{
	Circle, 
	//AABB,
	UI/*Rect*/
};

class Collider
{
public:
	Vector2 position;
	float radius;
	int width;
	int height;

public:
	Collider();
	Collider(const Vector2& position, float radius);
	Collider(const Vector2& position, int width, int height);

	void UpdateCollider(const Vector2& position);
};
