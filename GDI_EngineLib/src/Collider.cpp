#include "pch.h"
#include "Collider.h"

#include "Vector2.h"

Collider::Collider()
	: height(0), width(0), radius(0)
{
}

Collider::Collider(const Vector2& position, float radius)
	: position(position), radius(radius), height(0), width(0)
{
}

Collider::Collider(const Vector2& position, int width, int height)
	: position(position), width(width), height(height), radius(0)
{
}

void Collider::UpdateCollider(const Vector2& position)
{
	this->position = position;
}