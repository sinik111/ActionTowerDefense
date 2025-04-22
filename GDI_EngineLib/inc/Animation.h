#pragma once

namespace Gdiplus
{
	class Bitmap;
	class Rect;
}

enum class SpriteType
{
	SingleRowSheet,
	MultiRowSheet,
	Single
};

class Animation
{
private:
	std::vector<Gdiplus::Bitmap*> m_Images;
	
	int m_Frames;

	int m_Rows;
	int m_Columns;

	float m_Duration;
	float m_CurrentTime;

	unsigned int m_Width;
	unsigned int m_Height;

	unsigned int m_FrameWidth;
	unsigned int m_FrameHeight;

	SpriteType m_SpriteType;

	bool m_IsLoop;
	bool m_IsEnd;

public:
	Animation(Gdiplus::Bitmap* image, int frames, float duration, bool isLoop);
	Animation(Gdiplus::Bitmap* image, int frames, int rows, int columns, float duration, bool isLoop);
	Animation(const std::vector<Gdiplus::Bitmap*>& images, int frames, float duration, bool isLoop);
	~Animation() = default;

public:
	void Play();

	void GetFrameInfo(Gdiplus::Bitmap*& out_image, Gdiplus::Rect& out_dst_rect, Gdiplus::Rect& out_src_rect) const;

	void Update();

	void Reset();

	bool IsEnd();
};