#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "SimpleMath.h"

#include <iostream>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class InputDevice;

class DisplayWin32 {
private:
	int clientHeight;
	int clientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;

public:
	DisplayWin32(LPCWSTR applicationName, HINSTANCE hInstance, int height, int width);

	HWND GetHwnd() { return hWnd; }
	int GetClientHeight() { return clientHeight; }
	int GetClientWidth() { return clientWidth; }
};