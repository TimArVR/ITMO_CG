#pragma once

#include "SimpleMath.h"
#include "Transform.h"

class Game;
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera(Game* game, Vector3 position, Vector3 direction);
	~Camera();

	Matrix getViewMatrix();
	Matrix getProjectionMatrix() const;

	void rotate(float dx, float dy);
	void translate(Vector3 translation);

private:
	Transform transform;

	Vector3 direction;

	float yaw = 0;
	float pitch = 0;

	float rotationSpeed = 0.05;
	float moveSpeed = 25; //camera movement speed
	
	Matrix projectionMatrix;
	
};

