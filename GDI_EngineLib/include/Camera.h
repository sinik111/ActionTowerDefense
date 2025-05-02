#pragma once

#include "Object.h"

class Camera
	: public Object
{
private:
	int m_Width;
	int m_Height;

public:
	Camera(const Vector2& position, int width, int height);
	~Camera() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	Vector2 GetPosition() const;
	int GetWidth() const;
	int GetHeight() const;
	Vector2 ToWorldView(const Vector2& position) const;
	Vector2 ToCameraView(const Vector2& position) const;
	void SetPosition(const Vector2& position);

	bool IsOutOfView(const Vector2& position, int width, int height) const;
	bool IsUIOutOfView(const Vector2& position, int width, int height) const;
};