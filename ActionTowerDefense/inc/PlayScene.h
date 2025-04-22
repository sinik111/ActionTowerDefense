#pragma once

#include "Scene.h"

enum class ResultCode;

class PlayScene :
	public Scene
{
public:
	PlayScene() = default;
	~PlayScene();

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