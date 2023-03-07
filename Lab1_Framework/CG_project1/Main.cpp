#include "Game.h"
#include "TriangleComponent.h"

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#include ".\SimpleMath.h"


int main()
{
	Game g;
	g.components.push_back(new TriangleComponent());
	g.Run();

}