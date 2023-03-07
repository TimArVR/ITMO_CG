#pragma once
#include <windows.h>

class DisplayWin32
{
public:
	DisplayWin32();


	int clientHeight{};
	int clientWidth{};
	HINSTANCE hInstance;
	LPCWSTR applicationName;
	HWND hWnd;
	WNDCLASSEX wc;

private:

};

