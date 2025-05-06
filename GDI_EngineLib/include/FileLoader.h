#pragma once

#include "Singleton.h"

namespace Gdiplus
{
	class Bitmap;
}

enum class ResultCode;

class FileLoader
	: public Singleton<FileLoader>
{
	friend class Singleton<FileLoader>;
private:
	std::wstring m_ResourcePath;
	std::string m_ResourcePathS;

private:
	FileLoader() = default;
	~FileLoader() = default;

public:
	ResultCode Initialize(const std::wstring& modulePath, const std::wstring& binaryFolderName, const std::wstring& resourceFolderName);

	std::wstring LoadTextFile(const std::wstring& fileName);
	Gdiplus::Bitmap* LoadImageFile(const std::wstring& fileName);
	
	std::string GetPath();
};