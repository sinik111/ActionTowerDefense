#pragma once

#include <ole2.h>
#include <gdiplus.h>

#include "Singleton.h"

enum class ResultCode;

class GDIRenderer
	: public Singleton<GDIRenderer>
{
	friend class Singleton<GDIRenderer>;

	using PenWidthMap = std::unordered_map<int, Gdiplus::Pen*>;

private:
	// 윈도우 정보
	HWND m_hWnd;
	int m_Width;
	int m_Height;

	// 버퍼
	HDC m_FrontBufferDC;
	HDC m_BackBufferDC;

	HBITMAP m_BackBufferBitmap;

	// GDI+
	ULONG_PTR m_GdiplusToken;
	Gdiplus::Graphics* m_pBackBufferGraphics;

	// 그리기 도구
	Gdiplus::FontFamily* m_pFontFamily;

	std::unordered_map<int, Gdiplus::Font*> m_pFonts;
	std::unordered_map<Gdiplus::ARGB, Gdiplus::SolidBrush*> m_pBrushes;
	std::unordered_map<Gdiplus::ARGB, PenWidthMap> m_pPens;

private: // 생성자, 소멸자
	GDIRenderer();
	~GDIRenderer() = default;

public: // 초기화, 정리
	ResultCode Initialize(HWND hWnd, int width, int height);
	void Shutdown();

public: // 그리기
	void BeginDraw() const;
	void DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dstRect);
	void DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dstRect, const Gdiplus::Rect& srcRect);
	void DrawRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawFillRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawFillRectangle(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawFillTriangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawFillTriangle(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawFillCircle(const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawFillCircle(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, const Gdiplus::Rect& rect);
	void DrawString(const wchar_t* text, const Gdiplus::Color& color, const Gdiplus::PointF& point, int size);
	void DrawString(Gdiplus::Graphics* graphics, const wchar_t* text, const Gdiplus::Color& color, const Gdiplus::PointF& point, int size);
	void DrawLine(const Gdiplus::Color& color, int width, const Gdiplus::Point& p1, const Gdiplus::Point& p2);
	void DrawLine(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, int width, const Gdiplus::Point& p1, const Gdiplus::Point& p2);
	void EndDraw() const;

public:
	int GetWidth() const;
	int GetHeight() const;
	HWND GetHWND() const;

private:
	Gdiplus::Font* GetFont(int size);
	Gdiplus::Pen* GetPen(const Gdiplus::Color& color, int size);
	Gdiplus::SolidBrush* GetBrush(const Gdiplus::Color& color);
};