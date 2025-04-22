#pragma once

#include <ole2.h>
#include <gdiplus.h>

#include "Singleton.h"

enum class ResultCode;

class Vector2;

class GDIRenderer
	: public Singleton<GDIRenderer>
{
	friend class Singleton<GDIRenderer>;

private:
	// ������ ����
	HWND m_hWnd;
	int m_Width;
	int m_Height;

	// ����
	HDC m_FrontBufferDC;
	HDC m_BackBufferDC;

	HBITMAP m_BackBufferBitmap;

	// GDI+
	ULONG_PTR m_GdiplusToken;
	Gdiplus::Graphics* m_pBackBufferGraphics;

private: // ������, �Ҹ���
	GDIRenderer();
	~GDIRenderer() = default;

public: // �ʱ�ȭ, ����
	ResultCode Initialize(HWND hWnd, int width, int height);
	void Shutdown();

public: // �׸���
	void BeginDraw() const;
	void DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dst_rect, const Gdiplus::Rect& src_rect) const;
	void DrawRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect) const;
	void DrawString(const wchar_t* text, const Gdiplus::Color& color, const Vector2& position, float size) const;
	void EndDraw() const;

public:
	int GetWidth() const;
	int GetHeight() const;
	HWND GetHWND() const;
};