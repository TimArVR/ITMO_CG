#pragma once

#include "GameObject.h"

class BoxObject : public GameObject
{

public:
	BoxObject(Game* game, Vector3 position, Vector4 color, float boxSize, float boxSpeed);

	void preDraw();

	
private:
	float m_boxSize = 1;
	float m_boxSpeed = 1;
	float m_rotateDelta = 0;
};

