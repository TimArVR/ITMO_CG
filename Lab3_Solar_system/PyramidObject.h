#pragma once

#include "GameObject.h"

class PyramidObject : public GameObject
{

public:
	PyramidObject(Game* game, Vector3 position, Vector4 color);

	void preDraw();
private:
	float m_rotateDelta;
};

