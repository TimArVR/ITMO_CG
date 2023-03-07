#pragma once

#include "GameObject.h"

class SphereObject : public GameObject
{

public:
	SphereObject(Game* game, Vector3 position, Vector4 color, float radius, float boxSpeed);

	void preDraw();


private:
	float m_boxSize = 1;
	float m_boxSpeed = 1;
	float m_rotateDelta = 0;
};

