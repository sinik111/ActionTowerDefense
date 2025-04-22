# pragma once

constexpr float EPSILON = 0.0001f;

template <typename T>
inline T Clamp(T value, T min_val, T max_val)
{
	return (value < min_val) ? min_val : (value > max_val) ? max_val : value;
}

inline bool Contain(int pos_x, int pos_y, int rect_x, int rect_y, int rect_width, int rect_height)
{
	if (!(pos_x < rect_x || pos_y < rect_y ||
		pos_x > rect_x + rect_width || pos_y > rect_y + rect_height))
	{
		return false;
	}

	return true;
}