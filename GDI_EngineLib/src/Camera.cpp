#include "pch.h"
#include "Camera.h"

Camera::Camera(const Vector2& position, int width, int height)
	: m_Width(width), m_Height(height)
{
	m_Position = position;
}

void Camera::Initialize()
{
}

void Camera::Destroy()
{
}

void Camera::Update()
{
}

void Camera::Render() const
{
}

Vector2 Camera::GetPosition() const
{
	return Vector2(m_Position.x - m_Width / 2, m_Position.y - m_Height / 2);
}

Vector2 Camera::ToCameraView(const Vector2& position) const
{
	return position - GetPosition();
}

void Camera::SetPosition(const Vector2& position)
{
	m_Position = position;
}
