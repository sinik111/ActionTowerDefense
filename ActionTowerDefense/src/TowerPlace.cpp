#include "framework.h"
#include "TowerPlace.h"

TowerPlace::TowerPlace(const Vector2& position)
{
	m_Position = position;
}

void TowerPlace::Initialize()
{
}

void TowerPlace::Destroy()
{
	__super::Destroy();
}

void TowerPlace::Update()
{
}

void TowerPlace::Render(const Camera& camera) const
{
}
