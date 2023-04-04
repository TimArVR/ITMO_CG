#include <iostream>
#include "Game.h"

using namespace DirectX::SimpleMath;

int main() {
	Game game(L"KatamariGame", 800, 800);

	KatamariBase player(&game, { 0, 0, 0 }, Quaternion::Identity, 1.5f);
	game._cameraController->_player = &player;

	Object ground(
		&game,
		{ 0, -1.5, 0 },
		Quaternion::Identity,
		"./Models/Carpet.glb",
		L"./Textures/concrete1.jpg",
		100.0f,
		1.6f,
		false
	);

	Object* object1 = new Object(
		&game,
		{ 3.5f,-0.5f,15.0f },
		Quaternion::CreateFromAxisAngle(Vector3::Up, XM_PI),
		"./Models/ToiletPapaer.obj",
		L"./Textures/Diffuse.png",
		0.5f,
		1.0f,
		true
	);
	object1->SetTarget(&player);


	Object* object2 = new Object(
		&game,
		{ 10.5f,-0.5f,7.0f },
		Quaternion::CreateFromAxisAngle(Vector3::Up, XM_PI),
		"./Models/RubixCube.obj",
		L"./Textures/Diffuse_Cube.png",
		0.5f,
		1.0f,
		true
	);
	object2->SetTarget(&player);

	Object* object3 = new Object(
		&game,
		{ 20.5f,-0.7f,12.0f },
		Quaternion::CreateFromAxisAngle(Vector3::Up, XM_PI),
		"./Models/newGroundSp.fbx",
		L"./Textures/newGroundSp.png",
		0.5f,
		1.0f,
		true
	);
	object3->SetTarget(&player);

	Object* object4 = new Object(
		&game,
		{ 30.5f,2.7f,20.0f },
		Quaternion::CreateFromAxisAngle(Vector3::Up, XM_PI),
		"./Models/mushroom.fbx",
		L"./Textures/mushroom.png",
		2.5f,
		1.5f,
		true
	);
	object4->SetTarget(&player);

	game.Run();
}