#include "framework.h"
#include "TowerButton.h"

#include "TowerPlace.h"

TowerButton::TowerButton(TowerPlace* pTargetTowerPlace)
	: m_pTargetTowerPlace(pTargetTowerPlace), m_ButtonIndex(0), m_pImages{}
{
	
}

void TowerButton::Initialize()
{
}

void TowerButton::Destroy()
{
	__super::Destroy();
}

void TowerButton::Update()
{
}

void TowerButton::Render(const Camera& camera) const
{
}
