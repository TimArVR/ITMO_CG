#include <d3dcompiler.h>
#include "Game.h"
#include "SimpleMath.h"
#include "GameObject.h"
#include "ConstantBuffer.h"

using namespace DirectX::SimpleMath;

GameObject::GameObject(Game* game, Vector3 position, Vector4 color) : m_game(game), m_color(color)
{
	transform = new Transform(position);
}

void GameObject::init(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices)
{
	HRESULT hr;

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = vertices.size() * sizeof(Vertex);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	m_game->device->CreateBuffer(
		&bd,
		&sd,
		pVertexBuffer.GetAddressOf()
	);

	stride = sizeof(Vertex);
	offset = 0u;
	m_game->context->IASetVertexBuffers(
		0u,
		1u,
		pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = indices.size() * sizeof(unsigned short);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	m_game->device->CreateBuffer(&ibd, &isd, pIndexBuffer.GetAddressOf());

	m_game->context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	m_game->device->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	);

	m_game->context->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	m_game->device->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);

	m_game->context->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_game->device->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	CD3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.StructureByteStride = 0u;
	hr = m_game->device->CreateBuffer(&cbd, NULL, &pConstantBuffer);

	//bind constant buffer to vertex shader
	m_game->context->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}

void GameObject::draw()
{
	preDraw();
	m_game->context->IASetVertexBuffers(
		0u,
		1u,
		pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	m_game->context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	m_game->context->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	m_game->context->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	m_game->context->IASetInputLayout(pInputLayout.Get());

	m_game->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_game->context->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	m_game->context->DrawIndexed(indicesCount, 0u, 0u);
}
