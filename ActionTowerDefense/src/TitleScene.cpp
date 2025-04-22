#include "framework.h"
#include "TitleScene.h"

#include <sstream>

#include "ResultCode.h"
#include "Debug.h"
#include "ResourceManager.h"
#include "FileLoader.h"
#include "GDIRenderer.h"

ResultCode TitleScene::Load()
{
	Debug::Log("Load Title Scene - TitleScene::Load");

	ResultCode rc = ResultCode::OK;

	std::wstring data = FileLoader::Get().LoadTextFile(L"data/TitleSceneResourceList.txt");
	if (data == L"")
	{
		return ResultCode::FAIL;
	}

	rc = ResourceManager::Get().AddString(L"Title", L"TitleSceneResourceList", data);
	if (rc != ResultCode::OK)
	{
		return rc;
	}

	rc = LoadResources();
	if (rc != ResultCode::OK)
	{
		return rc;
	}

	return ResultCode::OK;
}

void TitleScene::Enter()
{
	Debug::Log("Enter Title Scene - TitleScene::Enter");
}

void TitleScene::Exit()
{
	Debug::Log("Exit Title Scene - TitleScene::Enter");

	__super::Exit();
}

void TitleScene::Unload()
{
	Debug::Log("Unload Title Scene - TitleScene::Enter");

	ResourceManager::Get().ReleaseResources(L"Title");
}

void TitleScene::Update()
{
	__super::Update();
}

ResultCode TitleScene::LoadResources()
{
	ResultCode rc = ResultCode::OK;

	std::wstring data = ResourceManager::Get().GetString(L"Title", L"TitleSceneResourceList");

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

