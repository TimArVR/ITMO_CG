#include "DirectionalLight.h"
#include "Game.h"

DirectionalLight::DirectionalLight(Game* game, int shadowSize, float radius, Vector4 lightColorAmb, Quaternion rotation) {
	localPosition = {0.0f, 0.0f, 0.0f};
	localScale = {1.0f, 1.0f, 1.0f};
	this->game = game;
	this->shadowSize = shadowSize;
	this->radius = radius;
	this->lightColorAmb = lightColorAmb;
	localRotation = rotation;

	D3D11_TEXTURE2D_DESC textDesc = {};
	textDesc.Width = shadowSize;
	textDesc.Height = shadowSize;
	textDesc.MipLevels = 1;
	textDesc.ArraySize = 1;
	textDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textDesc.SampleDesc.Count = 1;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.CPUAccessFlags = 0;
	textDesc.MiscFlags = 0;
	textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	auto res = game->device->CreateTexture2D(&textDesc, nullptr, &shadowMap);

	D3D11_TEX2D_DSV tex2DDSV = {};
	tex2DDSV.MipSlice = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	depthViewDesc.Texture2D = tex2DDSV;
	
	res = game->device->CreateDepthStencilView(shadowMap, &depthViewDesc, &depthStencilView);

	D3D11_TEX2D_SRV tex2DSRV = {};
	tex2DSRV.MipLevels = 1;
	tex2DSRV.MostDetailedMip = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc = {};
	shaderViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderViewDesc.Texture2D = tex2DSRV;

    res = game->device->CreateShaderResourceView(shadowMap, &shaderViewDesc, &shaderResourceView);
}

Matrix DirectionalLight::GetLightSpace() {
	return Matrix(GetView() * XMMatrixOrthographicLH(radius, radius, -radius / 2.0f, radius / 2.0f));
}
