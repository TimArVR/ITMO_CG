#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include "DisplayWin32.h"
#include "CameraController.h"
#include "KatamariBase.h"
#include "Camera.h"

using namespace std::chrono;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class DirectionalLight;
class InputDevice;

class Game {
private:
	float totalTime;
	float deltaTime;
	unsigned int frameCount;
	time_point<steady_clock> PrevTime;

	D3D11_VIEWPORT defaultViewPort = {};
	D3D11_VIEWPORT shadowsViewPort = {};
	
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11Texture2D> backTex;
	ComPtr<ID3D11InputLayout> layout;

	ComPtr<ID3D11PixelShader> lightPixelShader;
	ComPtr<ID3D11VertexShader> lightVertexShader;

	Matrix matrices[4];
	XMFLOAT4 vectors[3];
	ComPtr<ID3D11Buffer> lightFirstConstBuffer;
	ComPtr<ID3D11Buffer> lightSecondConstBuffer;

	ComPtr<ID3D11VertexShader> shadowsVertexShader;
	ComPtr<ID3D11Buffer> shadowsConstBuffer;

	void Initialize();
	void Update();
	void UpdateInternal();
	void RenderLight();
	void RenderShadows();

public:
	DisplayWin32* display;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	static InputDevice* inputDevice;

	std::vector<GameComponent*> components;

	DirectionalLight* directionalLight;
	Camera camera;
	CameraController* _cameraController;
	ComPtr<ID3D11RenderTargetView> rtv;

	ComPtr<ID3D11Texture2D> depthStencilTexture;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	Game(LPCWSTR applicationName, int clientHeight, int clientWidth);

	void Run();

	static LRESULT ProccesMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};