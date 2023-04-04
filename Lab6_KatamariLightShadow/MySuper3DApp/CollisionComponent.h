#pragma once
#include "DisplayWin32.h"

using namespace DirectX;

class Object;

class CollisionComponent {
protected:
	Object* target;
	bool hasParent = false;

public:
	BoundingSphere bSphere = {};

	bool CheckCollision();
	void SetBoundingShape(XMFLOAT3 center, float radius, Object* target = nullptr);
};
