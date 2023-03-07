#pragma once

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};