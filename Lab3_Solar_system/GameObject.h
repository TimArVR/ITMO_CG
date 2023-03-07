#pragma once
#include <d3d11.h>
#include "SimpleMath.h"
#include "Vertex.h"
#include <wrl/client.h>
#include "Transform.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

class Game;

class GameObject
{
public:
	GameObject(Game* game, Vector3 position, Vector4 color);
	virtual ~GameObject() = default;
	
	void init(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices);

	virtual void preDraw() = 0;
	void draw();

	Transform* transform;
	
protected:
	Game* m_game;

	ComPtr<ID3D11Buffer> pIndexBuffer;
	ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT indicesCount;
	
	ComPtr<ID3D11InputLayout> pInputLayout;
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_SUBRESOURCE_DATA* csd;
	
	Vector4 m_color;

	UINT stride;
	UINT offset = 0u;
};

