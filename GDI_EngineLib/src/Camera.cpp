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

void Camera::Render(const Camera& camera) const
{
}

Vector2 Camera::GetPosition() const
{
	return m_Position;
}

Vector2 Camera::ToCameraView(const Vector2& position) const
{
	return position - GetPosition();
}

void Camera::SetPosition(const Vector2& position)
{
	m_Position = Vector2(position.x - m_Width / 2, position.y - m_Height / 2);
}

bool Camera::IsOutOfView(const Vector2& position, int width, int height)
{
	return m_Position.x + m_Width < position.x - width / 2 || 
		m_Position.x > position.x + width / 2 ||
		m_Position.y + m_Height < position.y - height / 2 ||
		m_Position.y > position.y + height / 2;
}

int Camera::GetWidth() const
{
	return m_Width;
}

int Camera::GetHeight() const
{
	return m_Height;
}