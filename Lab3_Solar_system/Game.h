#pragma once
#include <d3d11_1.h>
#include "InputDevice.h"
#include "SimpleMath.h"
#include "Camera.h"
#include "Mouse.h"

using namespace DirectX::SimpleMath;

class Game
{
public:
	Game();
	~Game();

	virtual void init();
	
	float deltaTime = 0;
	float currentTime = 0;

	int screenWidth = 0;
	int screenHeight = 0;

	HWND hWnd;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3DUserDefinedAnnotation* annotation;
	ID3D11Texture2D* depthStencil = NULL;             // Depth Buffer Texture
	ID3D11DepthStencilView* depthStencilView = NULL;          // View object, depth buffer

	InputDevice* inputDevice;
	Mouse* mouse;
	
	void doFrame();

	Camera* camera;
	
protected:
	virtual void update();
	void render();
	virtual void drawObjects();
};

