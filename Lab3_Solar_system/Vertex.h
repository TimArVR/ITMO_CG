#pragma once

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Vertex
{
public:
	Vector3 pos;
	Vector4 color;
public:
	Vertex() = default;
	Vertex(Vector3 p, Vector4 c) : pos(p), color(c) {
		
	}
	Vertex operator+(Vertex const v) {
		Vertex vNew;
		vNew.pos.x = v.pos.x + pos.x;
		vNew.pos.y = v.pos.y + pos.y;
		vNew.color = color;
		return vNew;
	}

	Vertex operator*(float const v) {
		Vertex vNew;
		vNew.pos.x = pos.x * v;
		vNew.pos.y = pos.y * v;
		vNew.color = color;
		return vNew;
	}
};