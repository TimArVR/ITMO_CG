#pragma once
#include "Object.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class KatamariBase : public Object {
private:
	bool shouldInter = false;
	float riseAmmount = 0.0f;

public:
	BoundingSphere boundigSphere;

	KatamariBase(Game* g, XMFLOAT3 objectPostition, Quaternion objectRotation, float radius);

	void Update(float deltaTime) override;
	void UpdateBoundingSphere();
	void Interpolate(float rate);
};