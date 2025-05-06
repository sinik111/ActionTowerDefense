#include "pch.h"
#include "FileLoader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <ole2.h>
#include <gdiplus.h>

#include "ResultCode.h"
#include "Debug.h"

#pragma comment(lib, "gdiplus.lib")

ResultCode FileLoader::Initialize(const std::wstring& modulePath, const std::wstring& binaryFolderName, const std::wstring& resourceFolderName)
{
	bool wrongDirectory = true;

	m_ResourcePath = modulePath;

	size_t pos = m_ResourcePath.length();

	while (pos != std::wstring::npos)
	{
		pos = m_ResourcePath.rfind(L'\\', pos - 1);
		if (pos != std::wstring::npos)
		{
			std::wstring folderName = m_ResourcePath.substr(pos + 1);
			if (folderName == binaryFolderName)
			{
				m_ResourcePath = m_ResourcePath.substr(0, pos); // 폴더 이름 제거
				wrongDirectory = false;
				break;
			}

			m_ResourcePath = m_ResourcePath.substr(0, pos);
		}
	}

	if (wrongDirectory)
	{
		Debug::Log("Wrong Directory. - FileLoader::Initialize()");

		return ResultCode::FAIL;
	}

	m_ResourcePath += std::wstring(L"\\" + resourceFolderName + L"\\");

	int size = WideCharToMultiByte(CP_ACP, 0, m_ResourcePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_ACP, 0, m_ResourcePath.c_str(), -1, &result[0], size, nullptr, nullptr);
	result.pop_back(); // null 문자 제거

	m_ResourcePathS = result;

	return ResultCode::OK;
}

std::wstring FileLoader::LoadTextFile(const std::wstring& fileName)
{
	std::wstring filePath = m_ResourcePath + fileName;

	std::wifstream stream(filePath);

	if (!stream.is_open())
	{
		Debug::Log(L"File load failed: " + fileName + L" FileLoader::LoadTextFile()");

		return L"";
	}

	std::wostringstream oss;

	oss << stream.rdbuf();

	stream.close();

	return oss.str();
}

Gdiplus::Bitmap* FileLoader::LoadImageFile(const std::wstring& fileName)
{
	std::wstring filePath = m_ResourcePath + fileName;

	Gdiplus::Bitmap* image = new Gdiplus::Bitmap(filePath.c_str());

	if (image->GetLastStatus())
	{
		delete image;

		Debug::Log(L"File load failed: " + fileName + L" FileLoader::LoadImageFile()");

		return nullptr;
	}

	return image;
}

std::string FileLoader::GetPath()
{
	return m_ResourcePathS;
}
