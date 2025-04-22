#include "pch.h"
#include "Object.h"

#include "RenderManager.h"

Object::Object()
	: m_IsDestroyed(false), m_RenderLayer(RenderLayer())
{

}

void Object::Destroy()
{
	m_IsDestroyed = true;
}

void Object::Update()
{
	if (!m_IsDestroyed)
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

const Collider& Object::GetCollider() const
{
	return m_Collider;
}

bool Object::IsDestroyed()
{
	return m_IsDestroyed;
}

void Object::Collide(Object& object, const std::wstring& groupName)
{

}

const Vector2& Object::GetPosition() const
{
	return m_Position;
}
