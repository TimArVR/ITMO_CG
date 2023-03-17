#include "RenderComponent.h"

#include <WICTextureLoader.h>
#include "Game.h"
#include "DisplayWin32.h"
#include "Camera.h"
#include "RenderSystem.h"
#include "GameObject.h"

using namespace DirectX;//BeLight
using namespace SimpleMath;//BeLight


RenderComponent::RenderComponent(std::string shaderFileName, D3D_PRIMITIVE_TOPOLOGY topology) : Component()
{
	this->isTexture = false; //
	this->shaderFileName = shaderFileName;
	this->topology = topology;
}

RenderComponent::RenderComponent(std::string shaderFileName, std::string textureFileName, D3D_PRIMITIVE_TOPOLOGY topology) : Component()
{
	this->isTexture = true; //
	this->textureFileName = textureFileName; //
	this->shaderFileName = shaderFileName;
	this->topology = topology;
}

RenderComponent::~RenderComponent()
{
	Game::GetInstance()->GetRenderSystem()->RemoveRenderComponent(this);
}

void RenderComponent::Initialize()
{
	Game::GetInstance()->GetRenderSystem()->renderComponents.push_back(this);

	std::wstring fileName(shaderFileName.begin(), shaderFileName.end());

	ID3DBlob* errorCode = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderByteCode;
	auto res = D3DCompileFromFile(
		fileName.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		vertexShaderByteCode.GetAddressOf(),
		&errorCode
	);
	if (FAILED(res))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorCode)
		{
			const char* compileErrors = (char*)(errorCode->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << shaderFileName << std::endl;
		}
		return;
	}
	Game::GetInstance()->GetRenderSystem()->device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, vertexShader.GetAddressOf()
	);

	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderByteCode;
	res = D3DCompileFromFile(
		fileName.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		pixelShaderByteCode.GetAddressOf(),
		&errorCode
	);
	Game::GetInstance()->GetRenderSystem()->device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, pixelShader.GetAddressOf()
	);

	if (isTexture) //
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			D3D11_INPUT_ELEMENT_DESC {
				"TEXCOORD", //
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			D3D11_INPUT_ELEMENT_DESC { //BeLight
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
			0}

		};
		Game::GetInstance()->GetRenderSystem()->device->CreateInputLayout(
			inputElements,
			//2, //BeLight
			std::size(inputElements),
			vertexShaderByteCode->GetBufferPointer(),
			vertexShaderByteCode->GetBufferSize(),
			inputLayout.GetAddressOf()
		);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			D3D11_INPUT_ELEMENT_DESC {
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			D3D11_INPUT_ELEMENT_DESC { //BeLight
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
			0}
		};

		Game::GetInstance()->GetRenderSystem()->device->CreateInputLayout(
			inputElements,
			//2, //BeLight
			std::size(inputElements),
			vertexShaderByteCode->GetBufferPointer(),
			vertexShaderByteCode->GetBufferSize(),
			inputLayout.GetAddressOf()
		);
	}

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Game::GetInstance()->GetRenderSystem()->device->CreateBuffer(&vertexBufDesc, &vertexData, vertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	//indexBufDesc.ByteWidth = sizeof(int) * std::size(indices); //BeLight
	indexBufDesc.ByteWidth = sizeof(unsigned int) * std::size(indices);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Game::GetInstance()->GetRenderSystem()->device->CreateBuffer(&indexBufDesc, &indexData, indexBuffer.GetAddressOf());

	//D3D11_BUFFER_DESC constBufDesc = {}; //BeLight
	//constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	//constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//constBufDesc.CPUAccessFlags = 0;
	//constBufDesc.MiscFlags = 0;
	//constBufDesc.StructureByteStride = 0;
	//constBufDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	//Game::GetInstance()->GetRenderSystem()->device->CreateBuffer(&constBufDesc, nullptr, constBuffer.GetAddressOf());

	constBuffers = new ID3D11Buffer * [2]; //BeLight

	D3D11_BUFFER_DESC constBufDescPerObject = {}; //BeLight
	constBufDescPerObject.ByteWidth = sizeof(LightPerObject);
	constBufDescPerObject.Usage = D3D11_USAGE_DEFAULT;
	constBufDescPerObject.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDescPerObject.CPUAccessFlags = 0;
	constBufDescPerObject.MiscFlags = 0;
	constBufDescPerObject.StructureByteStride = 0;
	Game::GetInstance()->GetRenderSystem()->device->CreateBuffer(&constBufDescPerObject, nullptr, &constBuffers[0]);

	D3D11_BUFFER_DESC constBufDescPerScene = {}; //BeLight
	constBufDescPerScene.ByteWidth = sizeof(LightPerScene);
	constBufDescPerScene.Usage = D3D11_USAGE_DEFAULT;
	constBufDescPerScene.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDescPerScene.CPUAccessFlags = 0;
	constBufDescPerScene.MiscFlags = 0;
	constBufDescPerScene.StructureByteStride = 0;
	Game::GetInstance()->GetRenderSystem()->device->CreateBuffer(&constBufDescPerScene, nullptr, &constBuffers[1]);

	if (isTexture) //
	{
		std::wstring textureName(textureFileName.begin(), textureFileName.end());

		res = DirectX::CreateWICTextureFromFile(
			Game::GetInstance()->GetRenderSystem()->device.Get(),
			Game::GetInstance()->GetRenderSystem()->context.Get(),
			textureName.c_str(),
			texture.GetAddressOf(),
			textureView.GetAddressOf()
		);

		D3D11_SAMPLER_DESC samplerStateDesc = {};
		samplerStateDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		res = Game::GetInstance()->GetRenderSystem()->device->CreateSamplerState(&samplerStateDesc, samplerState.GetAddressOf());
	}

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.DepthClipEnable = true;
	Game::GetInstance()->GetRenderSystem()->device->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());

}

void RenderComponent::Update(float deltaTime)
{
	//UINT strides[] = { 32 }; //BeLight
	//UINT strides[] = { 48 }; //3 float4 //BeLight
	//UINT offsets[] = { 0 };
	//perObject = {};//BeLight
	//perScene = {};//BeLight
	perScene.lightDirection = { 1.0f, 1.0f, 0.0f, 0.0f };//BeLight
	perScene.lightDirection.Normalize();//BeLight
	perScene.lightColor = { 1.0f, 1.0f, 1.0f, 0.5f };//BeLight


	DirectX::XMMATRIX modelViewProjectionMatrix = gameObject->transformComponent->GetModel() *
		Game::GetInstance()->currentCamera->gameObject->transformComponent->GetView() * Game::GetInstance()->currentCamera->GetProjection();

	DirectX::XMMATRIX ScaledMatrix = DirectX::XMMatrixScaling(
		static_cast<float>(Game::GetInstance()->GetDisplay()->GetClientHeight())
		/ 
		static_cast<float>(Game::GetInstance()->GetDisplay()->GetClientWidth()),
		1.0f,
		1.0f
	);
	DirectX::XMMATRIX Transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(ScaledMatrix, modelViewProjectionMatrix));
	//Game::GetInstance()->GetRenderSystem()->context->UpdateSubresource(constBuffer.Get(), 0, nullptr, &Transform, 0, 0); //BeLight

	perObject.constBufMatrix = modelViewProjectionMatrix; //BeLight?? //Should be scale*rotation*position*Camera
	perObject.invertTransposeWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr,gameObject->transformComponent->GetModel()));  //BeLight?? //Should be scale*rotation invert transpose
	perScene.viewDirectionSpecular = //BeLight??
		Vector4(
			Game::GetInstance()->currentCamera->gameObject->transformComponent->GetPosition().x - Game::GetInstance()->currentCamera->target.x,
			Game::GetInstance()->currentCamera->gameObject->transformComponent->GetPosition().y - Game::GetInstance()->currentCamera->target.y,
			Game::GetInstance()->currentCamera->gameObject->transformComponent->GetPosition().z - Game::GetInstance()->currentCamera->target.z,
			0.0f
		);
	perScene.viewDirectionSpecular.Normalize();//BeLight
	perScene.viewDirectionSpecular.w = 0.5f;//BeLight
	Game::GetInstance()->GetRenderSystem()->context->UpdateSubresource(constBuffers[0], 0, nullptr, &perObject, 0, 0);//BeLight
	Game::GetInstance()->GetRenderSystem()->context->UpdateSubresource(constBuffers[1], 0, nullptr, &perScene, 0, 0);//BeLight
	Game::GetInstance()->GetRenderSystem()->context->VSSetConstantBuffers(0, 2, constBuffers);//BeLight
	Game::GetInstance()->GetRenderSystem()->context->PSSetConstantBuffers(0, 2, constBuffers);//BeLight



}

void RenderComponent::Draw()
{
	Game::GetInstance()->GetRenderSystem()->context->RSSetState(rastState.Get());
	Game::GetInstance()->GetRenderSystem()->context->IASetInputLayout(inputLayout.Get());
	Game::GetInstance()->GetRenderSystem()->context->IASetPrimitiveTopology(topology);
	Game::GetInstance()->GetRenderSystem()->context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	////UINT strides[] = { 32 }; //BeLight
	UINT strides[] = { 48 }; //3 float4 //BeLight
	UINT offsets[] = { 0 };
	//perObject = {};//BeLight
	//perScene = {};//BeLight
	//perScene.lightDirection = { 1.0f, 1.0f, 0.0f, 0.0f };//BeLight
	//perScene.lightDirection.Normalize();//BeLight
	//perScene.lightColor = { 1.0f, 1.0f, 1.0f, 0.5f };//BeLight

	Game::GetInstance()->GetRenderSystem()->context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), strides, offsets);
	Game::GetInstance()->GetRenderSystem()->context->VSSetShader(vertexShader.Get(), nullptr, 0);
	Game::GetInstance()->GetRenderSystem()->context->PSSetShader(pixelShader.Get(), nullptr, 0);

	if (isTexture)
	{
		Game::GetInstance()->GetRenderSystem()->context->PSSetShaderResources(0, 1, textureView.GetAddressOf()); //
		Game::GetInstance()->GetRenderSystem()->context->PSSetSamplers(0, 1, samplerState.GetAddressOf()); //
	}
	//Game::GetInstance()->GetRenderSystem()->context->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());//BeLight
	Game::GetInstance()->GetRenderSystem()->context->DrawIndexed(indices.size(), 0, 0);



}

void RenderComponent::AddCube(float radius)
{
	points = {
		Vector4(   radius,   radius, 0.0f, 1.0f), Vector4(radius * 2, radius * 2, 0.0f, 0.0f),
		Vector4( - radius, - radius, 0.0f, 1.0f), Vector4( 0.0f,   0.0f,  0.0f, 0.0f),
		Vector4(   radius, - radius, 0.0f, 1.0f), Vector4(radius * 2,  0.0f,  0.0f, 0.0f),
		Vector4( - radius,   radius, 0.0f, 1.0f), Vector4( 0.0f,  radius * 2, 0.0f, 0.0f)
	};

	indices = { 0, 1, 2, 1, 0, 3 };

}
void RenderComponent::AddSphere(float radius, int sliceCount, int stackCount, DirectX::XMFLOAT4 color)
{
	if (isTexture)
	{
		points.push_back({ 0.0f, radius, 0.0f, 1.0f });
		points.push_back({ 0.0f, 0.0f, 0.0f, 0.0f, });
		points.push_back({ 0.0f, 0.0f, 0.0f, 0.0f, }); // Normal //BeLight

		const float phiStep = DirectX::XM_PI / static_cast<float>(stackCount);
		const float thetaStep = DirectX::XM_2PI / static_cast<float>(sliceCount);

		for (int i = 1; i <= stackCount - 1; i++) {
			const float phi = static_cast<float>(i) * phiStep;

			for (int j = 0; j <= sliceCount; j++) {
				const float theta = static_cast<float>(j) * thetaStep;
				
				Vector4 tempPoint = {};
				Vector4 tempTexCoords = {};

				tempPoint.x = radius * sinf(phi) * cosf(theta);
				tempPoint.y = radius * cosf(phi);
				tempPoint.z = radius * sinf(phi) * sinf(theta);
				tempPoint.w = 1.0f;

				tempTexCoords.x = theta / DirectX::XM_2PI;
				tempTexCoords.y = phi / DirectX::XM_2PI;

				points.push_back(tempPoint);
				points.push_back(tempTexCoords);
				points.push_back({ 0.0f, 0.0f, 0.0f, 0.0f, }); // Normal //BeLight
			}
		}

		points.push_back({ 0.0f, -radius, 0.0f, 1.0f });
		points.push_back({ 0.0f, 1.0f, 0.0f, 0.0f, });
		points.push_back({ 0.0f, 0.0f, 0.0f, 0.0f, }); // Normal //BeLight

		for (int i = 1; i <= sliceCount; i++) {
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		int baseIndex = 1;
		const int ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; i++) {
			for (int j = 0; j < sliceCount; j++) {
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//const int southPoleIndex = points.size() / 2 - 1; //BeLight
		const int southPoleIndex = points.size() / 3 - 1; //BeLight

		baseIndex = southPoleIndex - ringVertexCount;

		for (int i = 0; i < sliceCount; i++) {
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}
	}
	else //?? do we need it
	{
		int g = 0;
		points.push_back(DirectX::XMFLOAT4(0, radius, 0, 1));
		points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		auto phiStep = DirectX::XM_PI / stackCount;
		auto thetaStep = DirectX::XM_2PI / sliceCount;
		DirectX::XMFLOAT4 colorBase[] = { { 1.0f, 0.3f, 0.3f, 1.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, { 0.7f, 0.7f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.9f, 1.0f } };
		for (int i = 1; i <= stackCount - 1; i++)
		{
			auto phi = i * phiStep;
			for (int j = 0; j <= sliceCount; j++)
			{
				if (((j / 20 == g)) && (g < 4))
				{
					color = colorBase[g++];
				}
				auto theta = j * thetaStep;
				points.push_back(
					DirectX::XMFLOAT4(
						radius * sin(phi) * cos(theta),
						radius * cos(phi),
						radius * sin(phi) * sin(theta),
						1.0f)
				);
				points.push_back(color);
			}
			g = 0;
		}
		points.push_back(DirectX::XMFLOAT4(0, -radius, 0, 1));
		points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		for (int i = 1; i <= sliceCount; i++)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}
		auto baseIndex = 1;
		auto ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; i++)
		{
			for (int j = 0; j < sliceCount; j++)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		auto southPoleIndex = indices.size() - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (int i = 0; i < sliceCount; i++)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}
	}
}
void RenderComponent::AddGrid(int gridSize, float cellSize, Color color)
{
	int firstPointIndex = points.size() / 2;
	int nPoints = gridSize * 2 + 1;
	float offset = -(nPoints / 2) * cellSize;
	for (int i = 0; i < nPoints; i++)
	{
		for (int j = 0; j < nPoints; j++)
		{
			points.push_back(Vector4(cellSize * i + offset, 0, cellSize * j + offset, 1));
			points.push_back(color);

			if (i == nPoints / 2 && j == nPoints / 2)
			{
				continue;
			}
			if (j < nPoints - 1)
			{
				indices.push_back(firstPointIndex + i * nPoints + j);
				indices.push_back(firstPointIndex + i * nPoints + j + 1);
			}
			if (i < nPoints - 1)
			{
				indices.push_back(firstPointIndex + i * nPoints + j);
				indices.push_back(firstPointIndex + i * nPoints + j + nPoints);
			}
		}
	}
}


void RenderComponent::AddMesh(float scaleRate, std::string objFileName)
{
	Assimp::Importer importer;
	//const aiScene* pScene = importer.ReadFile("../Models/ToiletPapaer.obj", aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	const aiScene* pScene = importer.ReadFile(objFileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!pScene) { return; }

	ProcessNode(pScene->mRootNode, pScene, scaleRate);
}
void RenderComponent::ProcessNode(aiNode* node, const aiScene* scene, float scaleRate)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, scaleRate);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, scaleRate);
	}
}
void RenderComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene, float scaleRate)
{
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		DirectX::SimpleMath::Vector4 textureCoordinate = {};

		if (mesh->mTextureCoords[0])
		{
			textureCoordinate.x = (float)mesh->mTextureCoords[0][i].x;
			textureCoordinate.y = (float)mesh->mTextureCoords[0][i].y;
		}

		points.push_back({ mesh->mVertices[i].x * scaleRate, mesh->mVertices[i].y * scaleRate, mesh->mVertices[i].z * scaleRate, 1.0f});
		points.push_back(textureCoordinate);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}