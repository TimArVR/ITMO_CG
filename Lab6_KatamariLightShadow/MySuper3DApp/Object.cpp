#include "Object.h"
#include "Game.h"
#include "InputDevice.h"
#include <math.h>
#include <vector>
#include "WICTextureLoader.h"

Object::Object(
	Game* g,
	XMFLOAT3 objectPostition,
	Quaternion objectRotation,
	float radius,
	bool shoudUseTrigger
) {
	if (shoudUseTrigger) {
		SetBoundingShape(objectPostition, radius);
		this->shoudUseTrigger = shoudUseTrigger;
	}

	localRotation = objectRotation;
	localPosition = objectPostition;
	localScale = { 1.0f, 1.0f, 1.0f };
	game = g;
}

Object::Object(
	Game* g,
	XMFLOAT3 objectPostition,
	Quaternion objectRotation,
	std::string filePath,
	LPCWSTR texturePath,
	float scaleRate,
	float radius,
	bool shoudUseTrigger
) : Object(g, objectPostition, objectRotation, radius, shoudUseTrigger) {
	this->texturePath = texturePath;
	this->scaleRate = scaleRate;

	LoadObject(filePath);
	Initialize();
}


void Object::Initialize() {
	game->components.push_back(this);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * vertexes.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexes.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * indexes.size();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexes.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	game->device->CreateBuffer(&indexBufDesc, &indexData, &ib);

	CreateWICTextureFromFile(game->device.Get(), game->context.Get(), texturePath, &texture, &textureView);

	D3D11_SAMPLER_DESC samplerStateDesc = {};
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	game->device->CreateSamplerState(&samplerStateDesc, &samplerState);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	game->device->CreateRasterizerState(&rastDesc, &rastState);
}

void Object::Update(float deltaTime) {
	if (!parent && shoudUseTrigger) {
		if (CheckCollision())
			SetParent(target, false);
	}
}

void Object::SetTarget(Object* target) {
	this->target = target;
}

void Object::Render() {
	game->context->PSSetShaderResources(0, 1, &textureView);
	game->context->PSSetSamplers(0, 1, &samplerState);

	game->context->RSSetState(rastState);

	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	game->context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	game->context->DrawIndexed(indexes.size(), 0, 0);
}

void Object::LoadObject(std::string filePath) {
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!pScene)
		return;

	ProcessNode(pScene->mRootNode, pScene);
}

void Object::ProcessNode(aiNode* node, const aiScene* scene) {
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene);
}

void Object::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		// Vertex position
		vertexes.push_back(
			{
				mesh->mVertices[i].x * scaleRate,
				mesh->mVertices[i].y * scaleRate,
				mesh->mVertices[i].z * scaleRate,
				1.0f
			}
		);

		// Texture coordinates
		XMFLOAT4 textureCoordinate = {};

		if (mesh->mTextureCoords[0]) {
			textureCoordinate.x = (float)mesh->mTextureCoords[0][i].x;
			textureCoordinate.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertexes.push_back(textureCoordinate);

		// Normals
		vertexes.push_back(
			{
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z,
				0.0f
			}
		);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indexes.push_back(face.mIndices[j]);
	}
}
