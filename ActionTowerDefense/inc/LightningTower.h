#pragma once

#include "Tower.h"
#include "GDIRenderer.h"

class Camera;

class LightningTower :
	public Tower
{
public:
	LightningTower(const Vector2& position);
	~LightningTower() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};