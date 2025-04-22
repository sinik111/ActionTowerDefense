#pragma once

#include "Scene.h"

enum class ResultCode;

class TitleScene :
	public Scene
{
public:
	TitleScene() = default;
	~TitleScene() = default;

public:
	ResultCode Load() override;
	void Enter() override;
	void Exit() override;
	void Unload() override;

public:
	void Update() override;

private:
	ResultCode LoadResources();
};