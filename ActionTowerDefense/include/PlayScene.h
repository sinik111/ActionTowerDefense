#pragma once

#include "Scene.h"

enum class ResultCode;
class TileMap;
class MiniMap;

class PlayScene :
	public Scene
{
private:
	TileMap* m_pTileMap;
	MiniMap* m_pMiniMap;

public:
	PlayScene();
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

	void CreateEnterGates();
	void CreateTowerPlaces();
};