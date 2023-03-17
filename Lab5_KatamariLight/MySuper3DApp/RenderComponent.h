#pragma once
#include "Component.h"

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX::SimpleMath;

struct LightPerObject { //BeLight
    Matrix constBufMatrix;
    Matrix invertTransposeWorld;
    Matrix worldP;
};

struct LightPerScene { //BeLight
    Vector4 lightDirection;
    Vector4 lightColor;
    Vector4 viewDirectionSpecular;
};



class RenderComponent : public Component
{
public:

    RenderComponent(std::string shaderFileName, D3D_PRIMITIVE_TOPOLOGY topology);
    RenderComponent(std::string shaderFileName, std::string textureFileName, D3D_PRIMITIVE_TOPOLOGY topology);
    RenderComponent() = delete;
    ~RenderComponent();

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;

    void Draw();
    void AddGrid(int gridSize, float cellSize, Color color);
    void AddCube(float radius);
    void AddSphere(float radius, int sliceCount, int stackCount, DirectX::XMFLOAT4 color);
    void AddMesh(float scaleRate, std::string objFileName);

    void ProcessNode(aiNode* node, const aiScene* scene, float scaleRate);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, float scaleRate);

protected:

    std::string shaderFileName;
    D3D_PRIMITIVE_TOPOLOGY topology;
    std::vector<Vector4> points;
    std::vector<int> indices;

    bool isTexture; //

    Microsoft::WRL::ComPtr<ID3D11VertexShader>    vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>     pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>     inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>          vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>          indexBuffer;
    //Microsoft::WRL::ComPtr<ID3D11Buffer>          constBuffer; //BeLight
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;

    ID3D11Buffer** constBuffers; //BeLight

    LightPerObject perObject; //BeLight
    LightPerScene perScene; //BeLight


    std::string textureFileName; //
    Microsoft::WRL::ComPtr<ID3D11Resource> texture; //
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView; //
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState; //
};
