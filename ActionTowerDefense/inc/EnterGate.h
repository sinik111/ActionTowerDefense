#pragma once

#include "Object.h"

namespace Gdiplus
{
	class Bitmap;
}

struct EnemyCreateData
{
	float createTime;
	int type;

	EnemyCreateData(float createTime, int type)
		: createTime(createTime), type(type)
	{

	}
};

class Camera;

class EnterGate :
	public Object
{
private:
	Gdiplus::Bitmap* m_pImage;
	std::vector<EnemyCreateData> m_EnemyCreateDatas;
	std::vector<Vector2> m_MoveData;
	int m_GateNumber;
	int m_CreateIndex;
	
public:
	EnterGate(int gateNumber, int row, int column);
	~EnterGate() = default;

public:
	void Initialize() override;
	void Destroy() override;

public:
	void Update() override;
	void Render(const Camera& camera) const override;
};