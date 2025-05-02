#pragma once

#include "Tower.h"
#include "GDIRenderer.h"

class Camera;

class FireTower :
	public Tower
{
public:
	FireTower(const Vector2& position);
	~FireTower() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;

public:
	virtual void Upgrade() override;

private:
	bool NearestComparer(const Object* a, const Object* b);
};