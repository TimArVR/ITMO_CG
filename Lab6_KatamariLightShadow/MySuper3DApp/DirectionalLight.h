#pragma once
#include "Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Game;

class DirectionalLight : public Transform {
public:
	Game* game;
	Vector4 lightColorAmb;
	float radius;
	int shadowSize;

	ID3D11Texture2D* shadowMap;
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11DepthStencilView* depthStencilView;

	DirectionalLight(Game* game, int shadowSize, float radius, Vector4 lightColorAmb, Quaternion rotation);

	Matrix GetLightSpace();
};

