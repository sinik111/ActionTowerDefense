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

	// �׸��� ����
	Gdiplus::Pen* m_pPen;
	Gdiplus::Pen* m_pLinePen;
	Gdiplus::FontFamily* m_pFontFamily;
	std::unordered_map<int, Gdiplus::Font*> m_pFonts;
	Gdiplus::SolidBrush* m_pBrush;

private: // ������, �Ҹ���
	GDIRenderer();
	~GDIRenderer() = default;

public: // �ʱ�ȭ, ����
	ResultCode Initialize(HWND hWnd, int width, int height);
	void Shutdown();

public: // �׸���
	void BeginDraw() const;
	void DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dst_rect) const;
	void DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dst_rect, const Gdiplus::Rect& src_rect) const;
	void DrawRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect) const;
	void DrawString(const wchar_t* text, const Gdiplus::Color& color, const Vector2& position, int size);
	void DrawString(Gdiplus::Graphics* graphics, const wchar_t* text, const Gdiplus::Color& color, Gdiplus::PointF& point, int size);
	void DrawLine(const Gdiplus::Color& color, float width, const Vector2& p1, const Vector2& p2) const;
	void DrawLine(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, float width, const Gdiplus::PointF& p1, const Gdiplus::PointF& p2) const;
	void EndDraw() const;

public:
	int GetWidth() const;
	int GetHeight() const;
	HWND GetHWND() const;

private:
	Gdiplus::Font* GetFont(int size);
};