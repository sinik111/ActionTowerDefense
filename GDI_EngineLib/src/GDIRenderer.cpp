#include "pch.h"
#include "GDIRenderer.h"

#include <ole2.h>
#include <gdiplus.h>

#include "Debug.h"
#include "ResultCode.h"
#include "Vector2.h"

#pragma comment(lib, "gdiplus.lib")

GDIRenderer::GDIRenderer()
	: m_hWnd(nullptr), m_Width(0), m_Height(0), m_FrontBufferDC(nullptr), m_BackBufferDC(nullptr),
	m_BackBufferBitmap(nullptr), m_GdiplusToken(0), m_pBackBufferGraphics(nullptr), m_pPen(nullptr),
	m_pFontFamily(nullptr), m_pBrush(nullptr), m_pLinePen(nullptr)
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
	//m_pBackBufferGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
	m_pPen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0));

	m_pFontFamily = new Gdiplus::FontFamily(L"맑은 고딕");

	m_pLinePen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 1.0f);

	m_pBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0));

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

	if (m_pPen != nullptr)
	{
		delete m_pPen;
		m_pPen = nullptr;
	}

	if (m_pLinePen != nullptr)
	{
		delete m_pLinePen;
		m_pLinePen = nullptr;
	}

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

void GDIRenderer::DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dst_rect) const
{
	m_pBackBufferGraphics->DrawImage(image, dst_rect);
}

void GDIRenderer::DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dst_rect, const Gdiplus::Rect& src_rect) const
{
	m_pBackBufferGraphics->DrawImage(image, dst_rect, src_rect.X, src_rect.Y, src_rect.Width, src_rect.Height, Gdiplus::UnitPixel);
}

void GDIRenderer::DrawRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect) const
{
	m_pPen->SetColor(color);

	m_pBackBufferGraphics->DrawRectangle(m_pPen, rect);
}

void GDIRenderer::DrawString(const wchar_t* text, const Gdiplus::Color& color, const Vector2& position, int size)
{
	m_pBrush->SetColor(color);

	m_pBackBufferGraphics->DrawString(text, -1, GetFont(size), Gdiplus::PointF(position.x, position.y), m_pBrush);
}

void GDIRenderer::DrawLine(const Gdiplus::Color& color, float width, const Vector2& p1, const Vector2& p2) const
{
	m_pLinePen->SetColor(color);
	m_pLinePen->SetWidth(width);

	m_pBackBufferGraphics->DrawLine(m_pLinePen, Gdiplus::PointF(p1.x, p1.y), Gdiplus::PointF(p2.x, p2.y));
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
