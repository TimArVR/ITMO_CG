#include "CameraController.h"
#include "Game.h"
#include "KatamariBase.h"
#include "Transform.h"

CameraController::CameraController(Game* game) {
	this->game = game;
	_player = nullptr;
	len = 8;
	minPitch = 5.0f / 180.0f * DirectX::XM_PI;
	maxPitch = 85.0f / 180.0f * DirectX::XM_PI;
	pitch = minPitch;
	yaw = 0;

	this->game->inputDevice->MouseMove.AddRaw(this, &CameraController::MouseEventHandler);
	camera = &game->camera;
}

void CameraController::Update(float deltaTime) {
	if (len < _player->boundigSphere.Radius)
		len += 0.1f;

	
	XMVECTOR arm = XMVector3TransformNormal({ 0.0f, 0.0f, -len }, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, XMLoadFloat3(&_player->localPosition) + arm);
	camera->SetPosition(temp);
	camera->SetRotation(Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
}

void CameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData) {
	pitch += mouseData.Offset.y * 0.005f;
	yaw += mouseData.Offset.x * 0.005f;

	len -= 0.005f * mouseData.WheelDelta;

	if (pitch < minPitch)
		pitch = minPitch;
	else if (pitch > maxPitch)
		pitch = maxPitch;
}
