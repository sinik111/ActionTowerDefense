#pragma once

#include "Object.h"

class TowerPlace :
	public Object
{
public:
	TowerPlace(const Vector2& position);
	~TowerPlace() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};