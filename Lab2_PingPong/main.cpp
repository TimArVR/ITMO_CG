
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <iostream>
#include "winuser.h"
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>
#include "Game.h"

#define ZCHECK(exp) if(FAILED(exp)) { printf("Check failed at file: %s at line %i", __FILE__, __LINE__); return 0; }

using namespace Microsoft::WRL;
using namespace DirectX;

Game* game;


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_SIZE:
	{

		return 0;
	}

	case WM_PAINT:
	{
		// Calculate current time and delta time	
		static float t = 0.0f;
		static DWORD dwTimeStart = GetTickCount();
		DWORD dwTimeCur = GetTickCount();
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
		game->deltaTime = t - game->currentTime;
		game->currentTime = t;
			
		// Main game loop
		game->doFrame();
			
		return 0;
	}

	// Input Device
	case WM_KILLFOCUS:
		game->inputDevice->ClearState();
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lparam & 0x40000000) || game->inputDevice->AutorepeatIsEnabled())
		{
			game->inputDevice->OnKeyPressed(static_cast<unsigned char>(wparam));
		}
		game->inputDevice->OnKeyPressed(static_cast<unsigned char>(wparam));

		// If a key is pressed send it to the input object so it can record that state.

		if ((unsigned int)wparam == 27) PostQuitMessage(0);
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		game->inputDevice->OnKeyReleased(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		game->inputDevice->OnChar(static_cast<unsigned char>(wparam));
		break;
	
	//End Input Device

	case WM_LBUTTONDOWN:
	{
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
#pragma region Game Initialization
	game = new Game();
	
#pragma region Window Initialization
	LPCWSTR applicationName;

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;



	// Give the application a name.
	applicationName = L"PingPongGame";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the screen.
	game->screenWidth = GetSystemMetrics(SM_CXSCREEN);
	game->screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// If windowed then set it to 800x600 resolution.
	game->screenWidth = 800;
	game->screenHeight = 600;

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - game->screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - game->screenHeight) / 2;

	RECT windowRect = { 0, 0,
		static_cast<LONG>(game->screenWidth), static_cast<LONG>(game->screenHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	
	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME; // WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
	// Create the window with the screen settings and get the handle to it.
	game->hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);


	ShowWindow(game->hWnd, SW_SHOW);
	SetForegroundWindow(game->hWnd);
	SetFocus(game->hWnd);

	ShowCursor(true);

#pragma endregion Window Initialization

#pragma region DirectX initialization
	HRESULT res;

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = game->screenWidth;
	swapDesc.BufferDesc.Height = game->screenHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = game->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
	res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_DEBUG, featureLevel, 1, D3D11_SDK_VERSION,
		&swapDesc, &game->swapChain, &game->device, nullptr, &game->context);
	ZCHECK(res);

	ID3D11Texture2D* backTex;
	res = game->swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backTex);
	ZCHECK(res);
	res = game->device->CreateRenderTargetView(backTex, nullptr, &game->rtv);
	ZCHECK(res);

	game->context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&game->annotation);

	ID3D11Debug* debug;
	game->device->QueryInterface(IID_ID3D11Debug, (void**)&debug);		

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rastState;
	res = game->device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);

	game->context->RSSetState(rastState);


	D3D11_VIEWPORT viewport = {};
	viewport.Width = game->screenWidth;
	viewport.Height = game->screenHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context->RSSetViewports(1, &viewport);
	game->context->OMSetRenderTargets(1, &game->rtv, nullptr);

#pragma endregion DirectX initialization

	MSG msg = {};

	game->init();

	// Loop until there is a quit message from the window or the user.
	bool isExitRequested = false;
	while (!isExitRequested) {
		
		// Handle the windows messages.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}
	}

	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	return 0;
}
