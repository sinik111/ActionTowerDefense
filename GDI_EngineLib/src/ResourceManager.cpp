#include "pch.h"
#include "ResourceManager.h"

#include <ole2.h>
#include <gdiplus.h>
#include <sstream>

#include "Debug.h"
#include "ResultCode.h"
#include "FileLoader.h"

ResultCode ResourceManager::AddImage(const std::wstring& group, const std::wstring& name, Gdiplus::Bitmap* image)
{
	if (image == nullptr)
	{
		Debug::Log(L"image is nullptr. " + group + L"/" + name + L" - ResourceManager::AddImage()");

		return ResultCode::FAIL;
	}

	Images& images = m_ImageGroups[group];

	if (images.find(name) != images.end())
	{
		Debug::Log(L"same name " + group + L"/" + name + L" - ResourceManager::AddImage()");

		return ResultCode::FAIL;
	}

	images.emplace(name, image);

	return ResultCode::OK;
}

Gdiplus::Bitmap* ResourceManager::GetImage(const std::wstring& group, const std::wstring& name)
{
	Images& images = m_ImageGroups[group];

	if (images.find(name) == images.end())
	{
		Debug::Log(L"can't find image. " + group + L"/" + name + L" - ResourceManager::GetImage()");

		return nullptr;
	}

	return images[name];
}

ResultCode ResourceManager::AddString(const std::wstring& group, const std::wstring& name, const std::wstring& data)
{
	Strings& strings = m_StringGroups[group];

	if (strings.find(name) != strings.end())
	{
		Debug::Log(L"same name " + group + L"/" + name + L" - ResourceManager::AddString()");

		return ResultCode::FAIL;
	}

	strings.emplace(name, data);

	return ResultCode::OK;
}

std::wstring ResourceManager::GetString(const std::wstring& group, const std::wstring& name)
{
	Strings& strings = m_StringGroups[group];

	if (strings.find(name) == strings.end())
	{
		Debug::Log(L"can't find image. " + group + L"/" + name + L" - ResourceManager::GetString()");

		return L"";
	}

	return strings[name];
}

ResultCode ResourceManager::AddByData(const std::wstring& data)
{
	ResultCode rc = ResultCode::OK;

	std::wstringstream wss(data);

	std::wstring groupName, resourceType, resourceName, fileName;
	while (!wss.eof())
	{
		wss >> groupName >> resourceType >> resourceName >> fileName;

		if (resourceType == L"Image")
		{
			Gdiplus::Bitmap* image = FileLoader::Get().LoadImageFile(fileName);
			if (image == nullptr)
			{
				return ResultCode::FAIL;
			}

			rc = ResourceManager::Get().AddImage(groupName, resourceName, image);
			if (rc != ResultCode::OK)
			{
				return rc;
			}
		}
		else if (resourceType == L"Text")
		{
			std::wstring data = FileLoader::Get().LoadTextFile(fileName);
			if (data == L"")
			{
				return ResultCode::FAIL;
			}

			rc = ResourceManager::Get().AddString(groupName, resourceName, data);
			if (rc != ResultCode::OK)
			{
				return rc;
			}
		}
	}

	return ResultCode::OK;
}

void ResourceManager::ReleaseResources(const std::wstring& group)
{
	Images& images = m_ImageGroups[group];

	for (auto pair : images)
	{
		delete pair.second;
	}

	images.clear();

	m_StringGroups[group].clear();
}