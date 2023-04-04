#pragma once
#include <DirectXMath.h>
#include "Transform.h"

using namespace DirectX;

class Camera : public Transform
{
public:
	Camera();
	void SetProjectionValuesPerspective(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	void SetProjectionValuesOrthographic(float aspectRatio, float height, float nearZ, float farZ);
	void SetIsPerspective(bool type);
	XMMATRIX GetProjectionMatrix() const;
	float fov;
	float nearZ;
	float farZ;
	float aspectRatio;
	float height;

private:
	bool isPerspective;
};
