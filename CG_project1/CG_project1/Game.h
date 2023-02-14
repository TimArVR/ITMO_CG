#pragma once

#include <vector>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <iostream>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


#include ".\DisplayWin32.h"

class GameComponent;

class Game
{
public:
	Game(); 
	void CreateBackBuffer();
	void DestroyResources();
	void Draw();
	void EndFrame();
	void Exit();	
	void Initialize();
	void MessageHandler();
	void PrepareFrame();
	void PrepareResources();
	void RestoreTargets();
	void Run();
	void Update();
	void UpdateInternal();


	DisplayWin32 display;
	std::vector<GameComponent*> components;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* backTex;
	ID3D11RenderTargetView* rtv;

	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;



private:
};

