#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "CollisionComponent.h"
#include "GameComponent.h"
#include "DisplayWin32.h"
#include "Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Object : public GameComponent, public Transform, public CollisionComponent {
private:
	LPCWSTR texturePath;
	float scaleRate;
	bool shoudUseTrigger;

	ID3D11RasterizerState* rastState;
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	ID3D11SamplerState* samplerState;
	ID3D11Buffer* ib;
	ID3D11Buffer* vb;
	UINT strides[1] = { 48 };
	UINT offsets[1] = { 0 };

	std::vector<XMFLOAT4> vertexes;
	std::vector<int> indexes;

	Object(
		Game* g,
		XMFLOAT3 objectPostition,
		Quaternion objectRotation,
		float radius,
		bool shoudUseTrigger = true
	);

public:
	Object(
		Game* g,
		XMFLOAT3 objectPostition,
		Quaternion objectRotation,
		std::string filePath,
		LPCWSTR texturePath,
		float scaleRate,
		float radius,
		bool shoudUseTrigger = true
	);

	void Initialize() override;
	void Update(float deltaTime) override;
	void Render();

	void LoadObject(std::string filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	void SetTarget(Object* target);
};
