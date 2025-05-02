#include "pch.h"
#include "GDIRenderer.h"

#include <ole2.h>
#include <gdiplus.h>

#include "Debug.h"
#include "ResultCode.h"

#pragma comment(lib, "gdiplus.lib")

GDIRenderer::GDIRenderer()
	: m_hWnd(nullptr), m_Width(0), m_Height(0), m_FrontBufferDC(nullptr), m_BackBufferDC(nullptr),
	m_BackBufferBitmap(nullptr), m_GdiplusToken(0), m_pBackBufferGraphics(nullptr),
	m_pFontFamily(nullptr)
{
	
}

ResultCode GDIRenderer::Initialize(HWND hWnd, int width, int height)
{
	Debug::Log("GDIRenderer::Initialize()");

	m_hWnd = hWnd;
	m_Width = width;
	m_Height = height;

	m_FrontBufferDC = GetDC(m_hWnd); // 윈도우 클라이언트 영역의 Device Context 얻기
	if (m_FrontBufferDC == nullptr)
	{
		Debug::Log("GetDC fail - GDIRenderer::Initialize()");

		return ResultCode::FAIL;
	}

	m_BackBufferDC = CreateCompatibleDC(m_FrontBufferDC); // 호환되는 Device Context 생성
	if (m_BackBufferDC == nullptr)
	{
		Debug::Log("CreateCompatibleDC fail - GDIRenderer::Initialize()");

		return ResultCode::FAIL;
	}

	m_BackBufferBitmap = CreateCompatibleBitmap(m_FrontBufferDC, width, height); // 메모리 영역 생성
	if (m_BackBufferBitmap == nullptr)
	{
		Debug::Log("CreateCompatibleBitmap fail - GDIRenderer::Initialize()");

		return ResultCode::FAIL;
	}

	SelectObject(m_BackBufferDC, m_BackBufferBitmap); // MemDC의 메모리 영역 지정

	Gdiplus::GdiplusStartupInput gsi;
	Gdiplus::Status status = Gdiplus::GdiplusStartup(&m_GdiplusToken, &gsi, nullptr);
	if (status != Gdiplus::Ok)
	{
		Debug::Log("Gdiplus::GdiplusStartup fail - GDIRenderer::Initialize()");

		return ResultCode::FAIL;
	}

	m_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(m_BackBufferDC);
	if (m_pBackBufferGraphics == nullptr)
	{
		Debug::Log("Gdiplus::Graphics::FromHD fail - GDIRenderer::Initialize()");

		return ResultCode::FAIL;
	}

	m_pBackBufferGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
	m_pBackBufferGraphics->SetInterpolationMode(Gdiplus::InterpolationModeLowQuality);

	m_pFontFamily = new Gdiplus::FontFamily(L"맑은 고딕");

	return ResultCode::OK;
}

void GDIRenderer::Shutdown()
{
	for (auto pair : m_pFonts)
	{
		delete pair.second;
	}
	m_pFonts.clear();

	if (m_pFontFamily != nullptr)
	{
		delete m_pFontFamily;
		m_pFontFamily = nullptr;
	}

	for (auto pens : m_pPens)
	{
		for (auto pair : pens.second)
		{
			delete pair.second;
		}
	}
	m_pPens.clear();

	for (auto pair : m_pBrushes)
	{
		delete pair.second;
	}
	m_pBrushes.clear();

	if (m_pBackBufferGraphics != nullptr)
	{
		delete m_pBackBufferGraphics;
		m_pBackBufferGraphics = nullptr;
	}

	if (m_GdiplusToken != 0)
	{
		Gdiplus::GdiplusShutdown(m_GdiplusToken);
		m_GdiplusToken = 0;
	}

	if (m_BackBufferBitmap != nullptr)
	{
		DeleteObject(m_BackBufferBitmap);
		m_BackBufferBitmap = nullptr;
	}

	if (m_BackBufferDC != nullptr)
	{
		DeleteDC(m_BackBufferDC);
		m_BackBufferDC = nullptr;
	}

	if (m_FrontBufferDC != nullptr)
	{
		ReleaseDC(m_hWnd, m_FrontBufferDC);
		m_FrontBufferDC = nullptr;
	}
}

void GDIRenderer::BeginDraw() const
{
	PatBlt(m_BackBufferDC, 0, 0, m_Width, m_Height, BLACKNESS);
}

void GDIRenderer::DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dstRect)
{
	m_pBackBufferGraphics->DrawImage(image, dstRect);
}

void GDIRenderer::DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dstRect, const Gdiplus::Rect& srcRect)
{
	m_pBackBufferGraphics->DrawImage(image, dstRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
}

void GDIRenderer::DrawRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	m_pBackBufferGraphics->DrawRectangle(GetPen(color, 1), rect);
}

void GDIRenderer::DrawFillRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	m_pBackBufferGraphics->FillRectangle(GetBrush(color), rect);
}

void GDIRenderer::DrawFillRectangle(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	graphics->FillRectangle(GetBrush(color), rect);
}

void GDIRenderer::DrawFillTriangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	Gdiplus::Point point[3] =
	{
		{ rect.X, rect.Y + rect.Height },
		{ rect.X + rect.Width, rect.Y + rect.Height },
		{ rect.X + rect.Width / 2, rect.Y }
	};

	m_pBackBufferGraphics->FillPolygon(GetBrush(color), point, 3);
}

void GDIRenderer::DrawFillTriangle(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	Gdiplus::Point point[3] =
	{
		{ rect.X, rect.Y + rect.Height },
		{ rect.X + rect.Width, rect.Y + rect.Height },
		{ rect.X + rect.Width / 2, rect.Y }
	};

	graphics->FillPolygon(GetBrush(color), point, 3);
}

void GDIRenderer::DrawFillCircle(const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	m_pBackBufferGraphics->FillEllipse(GetBrush(color), rect);
}

void GDIRenderer::DrawFillCircle(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, const Gdiplus::Rect& rect)
{
	graphics->FillEllipse(GetBrush(color), rect);
}

void GDIRenderer::DrawString(const wchar_t* text, const Gdiplus::Color& color, const Gdiplus::PointF& point, int size)
{
	m_pBackBufferGraphics->DrawString(text, -1, GetFont(size), point, GetBrush(color));
}

void GDIRenderer::DrawString(Gdiplus::Graphics* graphics, const wchar_t* text, const Gdiplus::Color& color, const Gdiplus::PointF& point, int size)
{
	graphics->DrawString(text, -1, GetFont(size), point, GetBrush(color));
}

void GDIRenderer::DrawLine(const Gdiplus::Color& color, int width, const Gdiplus::Point& p1, const Gdiplus::Point& p2)
{
	m_pBackBufferGraphics->DrawLine(GetPen(color, width), p1, p2);
}

void GDIRenderer::DrawLine(Gdiplus::Graphics* graphics, const Gdiplus::Color& color, int width, const Gdiplus::Point& p1, const Gdiplus::Point& p2)
{
	graphics->DrawLine(GetPen(color, width), p1, p2);
}

void GDIRenderer::EndDraw() const
{
	BitBlt(m_FrontBufferDC, 0, 0, m_Width, m_Height, m_BackBufferDC, 0, 0, SRCCOPY);
}

int GDIRenderer::GetWidth() const
{
	return m_Width;
}

int GDIRenderer::GetHeight() const
{
	return m_Height;
}

HWND GDIRenderer::GetHWND() const
{
	return m_hWnd;
}

Gdiplus::Font* GDIRenderer::GetFont(int size)
{
	auto iter = m_pFonts.find(size);
	if (iter != m_pFonts.end())
	{
		return iter->second;
	}

	Gdiplus::Font* pFont = new Gdiplus::Font(m_pFontFamily, (float)size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	m_pFonts[size] = pFont;

	return pFont;
}

Gdiplus::Pen* GDIRenderer::GetPen(const Gdiplus::Color& color, int size)
{
	Gdiplus::ARGB argb = color.GetValue();

	auto& penMap = m_pPens[argb];
	auto iter = penMap.find(size);
	if (iter != penMap.end())
	{
		return iter->second;
	}

	Gdiplus::Pen* pPen = new Gdiplus::Pen(color, (float)size);

	penMap[size] = pPen;

	return pPen;
}

Gdiplus::SolidBrush* GDIRenderer::GetBrush(const Gdiplus::Color& color)
{
	Gdiplus::ARGB argb = color.GetValue();

	auto iter = m_pBrushes.find(argb);
	if (iter != m_pBrushes.end())
	{
		return iter->second;
	}

	Gdiplus::SolidBrush* pBrush = new Gdiplus::SolidBrush(color);

	m_pBrushes[argb] = pBrush;

	return pBrush;
}
