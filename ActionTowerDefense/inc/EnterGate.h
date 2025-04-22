#pragma once

#include "Object.h"

class Camera;

class EnterGate :
	public Object
{
	
public:
	EnterGate() = default;
	~EnterGate() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};