#include "GameObject.h"

#include "Transform.h"
#include "RenderComponent.h"
#include "Component.h"

#include "Game.h"
#include "Camera.h"
#include "CameraController.h"

GameObject::GameObject(GameObject* parent)
{
	this->transformComponent = new TransformComponent();
	AddComponent(transformComponent);
	this->renderComponent = nullptr;
	this->collisionComponent = nullptr;
	this->radius = 0.0f;
}

GameObject::~GameObject()
{
	for (auto component : components)
	{
		delete component;
	}
	components.clear();
}

void GameObject::CreateSphere(float radius, int sliceCount, int stackCount, DirectX::XMFLOAT4 color)
{
	this->radius = radius;
	renderComponent = new RenderComponent("../Shaders/TexturedShader.hlsl", "../Textures/poll.bmp", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//renderComponent = new RenderComponent("../Shaders/MyVeryFirstShader.hlsl", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderComponent->AddSphere(radius, sliceCount, stackCount, color);
	AddComponent(renderComponent);
}

void GameObject::CreateCube(float radius)
{
	this->radius = radius;
	renderComponent = new RenderComponent("../Shaders/TexturedShader.hlsl", "../Textures/concrete.jpg", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//renderComponent = new RenderComponent("../Shaders/MyVeryFirstShader.hlsl", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderComponent->AddCube(radius);
	AddComponent(renderComponent);
}

void GameObject::CreateMesh(float scaleRate, std::string textureFileName, std::string objFileName)
{
	renderComponent = new RenderComponent("../Shaders/TexturedShader.hlsl", textureFileName, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderComponent->AddMesh(scaleRate, objFileName);
	AddComponent(renderComponent);
}

void GameObject::Initialize()
{
	for (const auto& component : components)
	{
		component->Initialize();
	}
}

void GameObject::Update(float deltaTime)
{
	for (const auto& component : components)
	{
		component->Update(deltaTime);
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->gameObject = this;
}