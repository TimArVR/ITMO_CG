#pragma once
#include "InputDevice.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class InputDevice;
class Game;
class Transform;
class KatamariBase;

class CameraController {
private:
	Transform* camera = nullptr;
	bool isInit = false;
	float len;
	float pitch = 0;
	float minPitch = 0;
	float maxPitch = 0;
	float yaw = 0;

public:
	Game* game;
	KatamariBase* _player;

	CameraController(Game* game);

	void Update(float deltaTime);
	void MouseEventHandler(const InputDevice::MouseMoveEventArgs& args);
};
