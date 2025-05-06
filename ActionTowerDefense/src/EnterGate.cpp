#include "framework.h"
#include "EnterGate.h"

#include <sstream>

#include "RenderManager.h"
#include "ResourceManager.h"
#include "GDIRenderer.h"
#include "Camera.h"
#include "GameData.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Constant.h"
#include "Random.h"
#include "Debug.h"

EnterGate::EnterGate(int gateNumber, int row, int column)
	: m_pImage(nullptr), m_GateNumber(gateNumber), m_CreateIndex(0), m_SpawnTimer(0.0f),
	m_IsSpawning(false), m_BurstCounter(0), m_BurstTimer(0.0f), m_BurstRate(0.5f)
{
	m_Position = Vector2(TILE_SIZE * row + (float)TILE_SIZE / 2,
		TILE_SIZE * column + (float)TILE_SIZE / 2);

	m_RenderLayer = RenderLayer::Object;
}

void EnterGate::Initialize()
{
	m_pImage = ResourceManager::Get().GetImage(L"Play", L"EnterGate");

	const std::wstring& moveData = ResourceManager::Get().GetString(L"Play", L"EnemyMoveData" + std::to_wstring(m_GateNumber));

	std::wstringstream wss(moveData);

	float x, y;
	while (wss >> x >> y)
	{
		m_MoveData.emplace_back(x * TILE_SIZE + TILE_SIZE / 2 , y * TILE_SIZE + TILE_SIZE / 2);
	}
}

void EnterGate::Destroy()
{
	__super::Destroy();
}

void EnterGate::Update()
{
	if (GameData::Get().GetCurrentGameState() == GameState::Play &&
		GameData::Get().GetRemainPlayTime() > 0)
	{
		if (!m_IsSpawning)
		{
			m_SpawnTimer += MyTime::DeltaTime();

			if (m_SpawnTimer > GameData::Get().GetCurrentSpawnRate())
			{
				m_IsSpawning = true;
				m_SpawnTimer = 0.0f;
			}
		}

		if (m_IsSpawning)
		{
			m_BurstTimer += MyTime::DeltaTime();

			if (m_BurstTimer > m_BurstRate)
			{
				SceneManager::Get().GetCurrentScene()->CreatePendingObject<Enemy>(m_Position, m_MoveData, (EnemyType)Random::Int(0, 2));
				m_BurstTimer = 0.0f;

				++m_BurstCounter;

				if (m_BurstCounter == GameData::Get().GetCurrentSpawnBurst())
				{
					m_IsSpawning = false;
					m_BurstCounter = 0;
				}
			}
		}
	}
	
	if (!SceneManager::Get().GetCurrentCamera()->IsOutOfView(m_Position, m_pImage->GetWidth(), m_pImage->GetHeight()))
	{
		RenderManager::Get().AddObject(m_RenderLayer, this);
	}
}

void EnterGate::Render(const Camera& camera) const
{
	Gdiplus::Rect dstRect, srcRect;
	srcRect.X = 0;
	srcRect.Y = 0;
	dstRect.Width = srcRect.Width = m_pImage->GetWidth();
	dstRect.Height = srcRect.Height = m_pImage->GetHeight();

	Vector2 cameraViewPos = camera.ToCameraView(m_Position);

	dstRect.X = (int)(cameraViewPos.x - srcRect.Width / 2);
	dstRect.Y = (int)(cameraViewPos.y - srcRect.Height / 2);

	GDIRenderer::Get().DrawImage(m_pImage, dstRect, srcRect);
}
