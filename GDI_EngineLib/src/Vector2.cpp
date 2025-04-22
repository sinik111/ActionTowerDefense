#include "pch.h"
#include "Vector2.h"

#include <cmath>
#include <string>

#include "MyMath.h"

const Vector2 Vector2::Zero{ 0.0f, 0.0f };
const Vector2 Vector2::One{ 1.0f, 1.0f };

const Vector2 Vector2::Up{ 0.0f, -1.0f };
const Vector2 Vector2::Down{ 0.0f, 1.0f };
const Vector2 Vector2::Right{ 1.0f, 0.0f };
const Vector2 Vector2::Left{ -1.0f, 0.0f };

Vector2::Vector2()
#ifdef _DEBUG
	: x(NAN), y(NAN)
#else
	: x(0.0f), y(0.0f)
#endif // _DEBUG
{
}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{
}

Vector2::Vector2(const Vector2& other)
	: x(other.x), y(other.y)
{
}

Vector2& Vector2::operator=(const Vector2& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;

	return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;

	return *this;
}

Vector2& Vector2::operator*=(float rhs)
{
	this->x *= rhs;
	this->y *= rhs;

	return *this;
}

Vector2& Vector2::operator/=(float rhs)
{
	if (fabs(rhs) < EPSILON)
	{
#ifdef _DEBUG
		this->x = NAN;
		this->y = NAN;
#else
		this->x = 0.0f;
		this->y = 0.0f;
#endif // _DEBUG

		return *this;
	}

	this->x /= rhs;
	this->y /= rhs;

	return *this;
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
	return Vector2(this->x + rhs.x, this->y + rhs.y);
}

Vector2 Vector2::operator-(const Vector2& rhs) const
{
	return Vector2(this->x - rhs.x, this->y - rhs.y);
}

Vector2 Vector2::operator*(float rhs) const
{
	return Vector2(this->x * rhs, this->y * rhs);
}

Vector2 Vector2::operator/(float rhs) const
{
	if (fabs(rhs) < EPSILON)
	{
#ifdef _DEBUG
		return Vector2(NAN, NAN);
#else
		return Vector2(0.0f, 0.0f);
#endif // _DEBUG
	}

	return Vector2(this->x / rhs, this->y / rhs);
}

float Vector2::SquareLength() const
{
	return this->x * this->x + this->y * this->y;
}

float Vector2::Length() const
{
	return sqrt(SquareLength());
}

Vector2 Vector2::Normalized() const
{
	float length = Length();
	if (length < EPSILON)
	{
#ifdef _DEBUG
		return Vector2(NAN, NAN);
#else
		return Vector2(0.0f, 0.0f);
#endif // _DEBUG
	}

	return *this / length;
}

void Vector2::Normalize()
{
	float length = Length();
	if (length < EPSILON)
	{
#ifdef _DEBUG
		this->x = NAN;
		this->y = NAN;
#else
		this->x = 0.0f;
		this->y = 0.0f;
#endif // _DEBUG

		return;
	}

	*this /= length;
}

std::string Vector2::ToString() const
{
	return std::string("( " + std::to_string(this->x) + ", " + std::to_string(this->y) + " )");
}

bool Vector2::IsZero() const
{
	return (fabs(this->x) < EPSILON && fabs(this->y) < EPSILON);
	//return (this->x == 0.0f && this->y == 0.0f);
}

float Vector2::AngleToUp() const
{
	constexpr float PI = 3.14159265f;

	Vector2 n = this->Normalized(); // 단위 벡터로 정규화

	// atan2(x, y): 위쪽 기준으로 시계방향 각도
	float radians = atan2(n.x, -n.y); // 주의: (x, y) 순서 (Up이 기준이므로)
	float degrees = radians * (180.0f / PI); // 라디안 → 도

	if (degrees < 0.0f)
	{
		degrees += 360.0f;
	}

	return degrees; // 0 ~ 360도 범위
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Distance(const Vector2& v1, const Vector2& v2)
{
	return (v1 - v2).Length();
}

float Vector2::SquareDistance(const Vector2& v1, const Vector2& v2)
{
	return (v1 - v2).SquareLength();
}

Vector2 Vector2::Direction(const Vector2& dst, const Vector2& src)
{
	return (dst - src).Normalized();
}

Vector2 Vector2::Lerp(const Vector2& v0, const Vector2& v1, float t)
{
	return Vector2(v0.x * (1.0f - t) + v1.x * t, v0.y * (1.0f - t) + v1.y * t);
}

Vector2 Vector2::Reflect(const Vector2& dir, const Vector2& normal)
{
	return Vector2(dir - normal * 2 * Dot(dir, normal));
}