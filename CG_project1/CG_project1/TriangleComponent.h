#pragma once
#include "./GameComponent.h"
//#include "./Game.h"


class TriangleComponent:public GameComponent
{
public:
	TriangleComponent();

	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Reload() override;
	void Update() override;

	ID3D11RasterizerState* rastState;
	ID3D11Buffer* ib;
	ID3D11Buffer* vb;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3DBlob* vertexBC;
	ID3DBlob* errorVertexCode;

	ID3DBlob* pixelBC;
	ID3DBlob* errorPixelCode;

	ID3D11InputLayout* layout;

	//DirectX::XMFLOAT4 points[8];
	D3D11_BUFFER_DESC vertexBufDesc;

private:

};

