#include "pch.h"
#include "Animation.h"

#include <ole2.h>
#include <gdiplus.h>

#include "MyTime.h"
#include "DebugUtility.h"
#include "MyMath.h"

Animation::Animation(Gdiplus::Bitmap* image, int frames, float duration, bool isLoop)
	: m_Frames(frames), m_Rows(0), m_Columns(0), m_Duration(duration),
	m_FrameHeight(0), m_IsLoop(isLoop), m_CurrentTime(0.0f),
	m_IsEnd(true), m_SpriteType(SpriteType::SingleRowSheet)
{
	m_Images.push_back(image);

	m_Width = image->GetWidth();
	m_Height = image->GetHeight();

	m_FrameWidth = m_Width / m_Frames;
}

Animation::Animation(Gdiplus::Bitmap* image, int frames, int rows, int columns, float duration, bool isLoop)
	: m_Frames(frames), m_Rows(rows), m_Columns(columns), m_Duration(duration),
	m_IsLoop(isLoop), m_CurrentTime(0.0f), m_IsEnd(true),
	m_SpriteType(SpriteType::MultiRowSheet)
{
	m_Images.push_back(image);

	m_Width = image->GetWidth();
	m_Height = image->GetHeight();

	m_FrameWidth = m_Width / m_Columns;
	m_FrameHeight = m_Height / m_Rows;
}

Animation::Animation(const std::vector<Gdiplus::Bitmap*>& images, int frames, float duration, bool isLoop)
	: m_Frames(frames), m_Rows(0), m_Columns(0), m_Duration(duration),
	m_IsLoop(isLoop), m_CurrentTime(0.0f), m_IsEnd(true),
	m_SpriteType(SpriteType::Single), m_FrameWidth(0), m_FrameHeight(0)
{
	m_Images = images;

	m_Width = images[0]->GetWidth();
	m_Height = images[0]->GetHeight();
}

void Animation::Play()
{
	m_IsEnd = false;
}

void Animation::GetFrameInfo(Gdiplus::Bitmap*& out_image, Gdiplus::Rect& out_dst_rect, Gdiplus::Rect& out_src_rect) const
{
	int frame = Clamp((int)(m_Frames * (m_CurrentTime / m_Duration)), 0, m_Frames - 1);

	switch (m_SpriteType)
	{
	case SpriteType::SingleRowSheet:
		out_image = m_Images[0];

		out_dst_rect.X = 0;
		out_dst_rect.Y = 0;
		out_dst_rect.Width = m_FrameWidth;
		out_dst_rect.Height = m_Height;

		out_src_rect.X = m_FrameWidth * frame;
		out_src_rect.Y = 0;
		out_src_rect.Width = m_FrameWidth;
		out_src_rect.Height = m_Height;
		break;

	case SpriteType::MultiRowSheet:
		out_image = m_Images[0];

		out_dst_rect.X = 0;
		out_dst_rect.Y = 0;
		out_dst_rect.Width = m_FrameWidth;
		out_dst_rect.Height = m_FrameHeight;

		out_src_rect.X = m_FrameWidth * (frame % m_Columns);
		out_src_rect.Y = m_FrameHeight * (frame / m_Columns);
		out_src_rect.Width = m_FrameWidth;
		out_src_rect.Height = m_FrameHeight;
		break;

	case SpriteType::Single:
		out_image = m_Images[frame];

		out_dst_rect.X = 0;
		out_dst_rect.Y = 0;
		out_dst_rect.Width = m_Width;
		out_dst_rect.Height = m_Height;

		out_src_rect.X = 0;
		out_src_rect.Y = 0;
		out_src_rect.Width = m_Width;
		out_src_rect.Height = m_Height;
		break;
	}
}

void Animation::Update()
{
	m_CurrentTime += MyTime::DeltaTime();
	if (m_CurrentTime >= m_Duration)
	{
		if (m_IsLoop)
		{
			m_CurrentTime -= m_Duration;
		}
		else
		{
			m_IsEnd = true;
		}
	}
}

void Animation::Reset()
{
	m_CurrentTime = 0.0f;
	m_IsEnd = true;
}

bool Animation::IsEnd()
{
	return m_IsEnd;
}

