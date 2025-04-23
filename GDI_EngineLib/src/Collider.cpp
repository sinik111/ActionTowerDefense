#include "pch.h"
#include "Collider.h"

#include "Vector2.h"

Collider::Collider()
	: height(0.0f), width(0.0f), type(ColliderType())
{
}

void Collider::SetColliderInfo(ColliderType type, const Vector2& position, const Vector2& offset, float width, float height)
{
	this->type = type;

	switch (type)
	{
	case ColliderType::Circle:
		this->position = position + offset;
		break;

	case ColliderType::AABB:
		this->position = Vector2(position.x - width / 2, position.y - height / 2) + offset;
		break;

	case ColliderType::UIRect:
		this->position = position;
		break;
	}

	this->offset = offset;
	this->width = width;
	this->height = height;
}

void Collider::UpdateCollider(const Vector2& position)
{
	switch (type)
	{
	case ColliderType::Circle:
		this->position = position + offset;
		break;

	case ColliderType::AABB:
		this->position = Vector2(position.x - width / 2, position.y - height / 2) + offset;
		break;

	case ColliderType::UIRect:
		this->position = position;
		break;
	}
}