#pragma once

#include <string>

class Vector2
{
public:
	float x;
	float y;

public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);

public:
	Vector2& operator=(const Vector2& rhs);
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2& operator*=(float rhs);
	Vector2& operator/=(float rhs);

public:
	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	Vector2 operator*(float rhs) const;
	Vector2 operator/(float rhs) const;

public:
	float SquareLength() const;

	float Length() const;

	Vector2 Normalized() const;

	void Normalize();

	std::string ToString() const;

	bool IsZero() const;

	float AngleToUp() const;

public:
	static float Dot(const Vector2& v1, const Vector2& v2);

	static float Distance(const Vector2& v1, const Vector2& v2);

	static float SquareDistance(const Vector2& v1, const Vector2& v2);

	static Vector2 Direction(const Vector2& dst, const Vector2& src);

	static Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t);

	static Vector2 Reflect(const Vector2& dir, const Vector2& normal);

public:
	static const Vector2 Zero;
	static const Vector2 One;

	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Right;
	static const Vector2 Left;
};