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
	m_BackBufferBitmap(nullptr), m_GdiplusToken(0), m_pBackBufferGraphics(nullptr)
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

	return ResultCode::OK;
}

void GDIRenderer::Shutdown()
{
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
	PatBlt(m_BackBufferDC, 0, 0, m_Width, m_Height, WHITENESS);
}

void GDIRenderer::DrawImage(Gdiplus::Bitmap* image, const Gdiplus::Rect& dst_rect, const Gdiplus::Rect& src_rect) const
{
	m_pBackBufferGraphics->DrawImage(image, dst_rect, src_rect.X, src_rect.Y, src_rect.Width, src_rect.Height, Gdiplus::UnitPixel);
}

void GDIRenderer::DrawRectangle(const Gdiplus::Color& color, const Gdiplus::Rect& rect) const
{
	Gdiplus::Pen pen(color);

	m_pBackBufferGraphics->DrawRectangle(&pen, rect);
}

void GDIRenderer::DrawString(const wchar_t* text, const Gdiplus::Color& color, const Vector2& position) const
{
	Gdiplus::Pen pen(color);

	Gdiplus::FontFamily fontFamily(L"맑은 고딕");
	Gdiplus::Font font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::SolidBrush brush(color);

	m_pBackBufferGraphics->DrawString(text, -1, &font, Gdiplus::PointF(position.x, position.y), &brush);
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
