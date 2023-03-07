#include "Game.h"
#include <iostream>

Game::Game()
{
	inputDevice = new InputDevice();
	mouse = new Mouse();
	mouse->EnableRaw();
}

Game::~Game()
{

}

void Game::init()
{
	
}

void Game::doFrame()
{
	update();
	render();
}

void Game::update()
{
	
}

void Game::render()
{
	float color[] = { 0.0f, 0.0f, 0.1f, 0.0f };
	context->ClearRenderTargetView(rtv, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	annotation->BeginEvent(L"BeginDraw");
	drawObjects();
	annotation->EndEvent();
	
	swapChain->Present(1, 0);
}

void Game::drawObjects()
{
	
}
