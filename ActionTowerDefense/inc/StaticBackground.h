#pragma once

#include "Object.h"

namespace Gdiplus
{
	class Bitmap;
}

class StaticBackground :
	public Object
{
private:
	Gdiplus::Bitmap* m_Image;
	int m_Width;
	int m_Height;

public:
	StaticBackground(Gdiplus::Bitmap* image);
	~StaticBackground() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render() const override;
};