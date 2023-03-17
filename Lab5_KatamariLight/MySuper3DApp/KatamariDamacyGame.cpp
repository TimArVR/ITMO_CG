#include "KatamariDamacyGame.h"
#include "DebugObject.h"
#include "Camera.h"
#include "CameraController.h"
#include "FPSCameraController.h"
#include "KatamariControllerComponent.h"
#include "CollisionComponent.h"
#include "Transform.h"


KatamariDamacyGame::KatamariDamacyGame(LPCWSTR name, int clientWidth, int clientHeight) : Game(name, clientWidth, clientHeight)
{
	Game::CreateInstance(name, clientWidth, clientHeight);
	Initialize();
}

void KatamariDamacyGame::Initialize()
{

	GameObject* floor = new GameObject();
	floor->CreateCube(100.0f);
	floor->transformComponent->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Right, DirectX::XM_PIDIV2));


	GameObject* katamari = new GameObject();
	katamari->CreateMesh(1.0f, "../Textures/Diffuse.png", "../Models/ToiletPapaer.obj");
	katamari->transformComponent->SetPosition(Vector3(0,1,0));
	katamari->radius = 1.0f;
	KatamariControllerComponent* katamariController = new KatamariControllerComponent();
	katamariController->katamariSpeed = 5.0f;
	katamari->AddComponent(katamariController);
	CollisionComponent* katamariCollision = new CollisionComponent();
	katamari->collisionComponent = katamariCollision;
	katamari->AddComponent(katamariCollision);

	GameObject* camera = new GameObject();
	CameraComponent* cameraComponent = new CameraComponent();
	CameraArmControllerComponent* armCameraController = new CameraArmControllerComponent();
	camera->AddComponent(cameraComponent);
	armCameraController->aim = katamari->transformComponent;
	camera->AddComponent(armCameraController);
	Game::GetInstance()->currentCamera = cameraComponent;
	katamariController->cameraTransform = camera->transformComponent;

	//Game::GetInstance()->AddGameObject(grid);
	Game::GetInstance()->AddGameObject(floor);
	Game::GetInstance()->AddGameObject(camera);
	Game::GetInstance()->AddGameObject(katamari);

	GameObject* objectN;
	CollisionComponent* objectCollisionN;

	objectN = new GameObject();
	objectN->CreateMesh(0.5f, "../Textures/Diffuse.png", "../Models/ToiletPapaer.obj");
	objectN->radius = 1.0f;
	objectN->maxRadius = objectN->radius;
	objectCollisionN = new CollisionComponent();
	objectN->collisionComponent = objectCollisionN;
	objectN->AddComponent(objectCollisionN);
	objectN->transformComponent->SetPosition(Vector3(3.5f,1.0f,15.0f));
	Game::GetInstance()->AddGameObject(objectN);

	objectN = new GameObject();
	objectN->CreateMesh(0.5f, "../Textures/Diffuse_Cube.png", "../Models/RubixCube.obj");
	objectN->radius = 1.0f;
	objectN->maxRadius = objectN->radius;
	objectCollisionN = new CollisionComponent();
	objectN->collisionComponent = objectCollisionN;
	objectN->AddComponent(objectCollisionN);
	objectN->transformComponent->SetPosition(Vector3(10.5f, 1.0f, 7.0f));
	Game::GetInstance()->AddGameObject(objectN);

	GameObject* object3 = new GameObject();
	object3->CreateMesh(0.5f, "../Textures/Diffuse_Cube.png", "../Models/RubixCube.obj");
	object3->radius = 1.0f;
	objectN->maxRadius = objectN->radius;
	CollisionComponent* objectCollision3 = new CollisionComponent();
	object3->collisionComponent = objectCollision3;
	object3->AddComponent(objectCollision3);
	object3->transformComponent->SetPosition(Vector3(15.5f, 1.0f, 5.0f));
	Game::GetInstance()->AddGameObject(object3);

	objectN = new GameObject();
	objectN->CreateMesh(0.5f, "../Textures/mushroom.png", "../Models/mushroom.fbx");
	objectN->radius = 1.0f;
	objectN->maxRadius = objectN->radius;
	objectCollisionN = new CollisionComponent();
	objectN->collisionComponent = objectCollisionN;
	objectN->AddComponent(objectCollisionN);
	objectN->transformComponent->SetPosition(Vector3(20.5f, 1.0f, 12.0f));
	Game::GetInstance()->AddGameObject(objectN);

	objectN = new GameObject();
	objectN->CreateMesh(2.5f, "../Textures/mushroom.png", "../Models/mushroom.fbx");
	objectN->radius = 1.5f;
	objectN->maxRadius = objectN->radius;
	objectCollisionN = new CollisionComponent();
	objectN->collisionComponent = objectCollisionN;
	objectN->AddComponent(objectCollisionN);
	objectN->transformComponent->SetPosition(Vector3(30.5f,4.0f, 20.0f));
	Game::GetInstance()->AddGameObject(objectN);

}

void KatamariDamacyGame::Run()
{
	Game::GetInstance()->Run();
}