#pragma once

#include "ResultCode.h"
#include "Vector2.h"
#include "Collider.h"

class Camera;
enum class RenderLayer;

class Object
{
protected:
	Vector2 m_Position;
	Collider m_Collider;
	bool m_IsDestroyed;
	RenderLayer m_RenderLayer;

public:
	Object();
	virtual ~Object() = default;

public:
	virtual void Initialize() = 0;
	virtual void Destroy();

public:
	virtual void Update();
	virtual void Render(const Camera& camera) const = 0;

public:
	const Collider& GetCollider() const;
	bool IsDestroyed();
	virtual void Collide(Object& object, const std::wstring& groupName);
	const Vector2& GetPosition() const;
};