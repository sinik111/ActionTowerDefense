#pragma once

#include "Singleton.h"

enum class ResultCode;

namespace Gdiplus
{
	class Bitmap;
}

class ResourceManager
	: public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

private:
	using Images = std::unordered_map<std::wstring, Gdiplus::Bitmap*>;
	using Strings = std::unordered_map<std::wstring, std::wstring>;

	std::unordered_map<std::wstring, Images> m_ImageGroups;
	std::unordered_map<std::wstring, Strings> m_StringGroups;

private:
	ResourceManager() = default;
	~ResourceManager() = default;

public:
	ResultCode AddImage(const std::wstring& group, const std::wstring& name, Gdiplus::Bitmap* image);
	Gdiplus::Bitmap* GetImage(const std::wstring& group, const std::wstring& name);

	ResultCode AddString(const std::wstring& group, const std::wstring& name, const std::wstring& data);
	std::wstring GetString(const std::wstring& group, const std::wstring& name);

public:
	void ReleaseResources(const std::wstring& group);
};