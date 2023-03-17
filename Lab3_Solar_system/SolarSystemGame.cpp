#include "SolarSystemGame.h"
#include <iostream>

using namespace DirectX::SimpleMath;

SolarSystemGame::SolarSystemGame() : Game()
{
	
}

SolarSystemGame::~SolarSystemGame()
{
}

void SolarSystemGame::init()
{
	camera = new Camera(this, {0, 0, -10}, {0, 0, 1});
	box1 = new SphereObject(this, {0,0,0}, {1,0.8,0,1}, 1.5f, 0.6f);
	box2 = new SphereObject(this, {6,0,0}, {0,1,0,1}, 0.5f, 1.0f);
	box2->transform->setParent(box1->transform);
	box3 = new SphereObject(this, {2,0,0}, {1,1,1,1}, 0.2f, 5.0f);
	box3->transform->setParent(box2->transform);
	box4 = new SphereObject(this, {10,0,0}, {1,1,1,1}, 0.7f, 0.3f);
	box5 = new SphereObject(this, { 14,0,0 }, { 1,0,1,1 }, 0.6f, 0.4f);
	box6 = new SphereObject(this, { 17,0,0 }, { 1,1,0.5,1 }, 0.4f, 0.7f);
	box7 = new SphereObject(this, { 18,0,0 }, { 0.5,1,0.5,1 }, 0.3f, 0.55f);
	box8 = new SphereObject(this, { 19,0,0 }, { 0.9,1,0.2,1 }, 0.25f, 0.45f);
	box9 = new SphereObject(this, { 20,0,0 }, { 0.6,0.8,0.4,1 }, 0.85f, 0.65f);
	box10 = new SphereObject(this, { 22,0,0 }, { 0.1,0.4,0.9,1 }, 0.95f, 0.75f);

	//camera = new Camera(this, { 0, 0, -10 }, { 0, 0, 1 });
	//box1 = new BoxObject(this, { 0,0,0 }, { 1,0.8,0,1 }, 1.5f, 0.6f);
	//box2 = new BoxObject(this, { 6,0,0 }, { 0,1,0,1 }, 0.5f, 1.0f);
	//box2->transform->setParent(box1->transform);
	//box3 = new BoxObject(this, { 2,0,0 }, { 1,1,1,1 }, 0.2f, 5.0f);
	//box3->transform->setParent(box2->transform);
	//box4 = new BoxObject(this, { 10,0,0 }, { 1,1,1,1 }, 0.7f, 0.3f);
	//box5 = new BoxObject(this, { 14,0,0 }, { 1,0,0,1 }, 0.6f, 0.4f);
	//box6 = new BoxObject(this, { 17,0,0 }, { 1,1,0.5,1 }, 0.4f, 0.7f);
	//box7 = new BoxObject(this, { 18,0,0 }, { 0.5,1,0.5,1 }, 0.3f, 0.55f);
	//box8 = new BoxObject(this, { 19,0,0 }, { 0.9,1,0.2,1 }, 0.25f, 0.45f);
	//box9 = new BoxObject(this, { 20,0,0 }, { 0.6,0.8,0.4,1 }, 0.85f, 0.65f);
	//box10 = new BoxObject(this, { 22,0,0 }, { 0.1,0.4,0.9,1 }, 0.95f, 0.75f);

}

void SolarSystemGame::update()
{
	while (const auto delta = mouse->ReadRawDelta())
	{
		camera->rotate((float)delta->x * deltaTime, (float)delta->y * -deltaTime);
	}

	if (inputDevice->KeyIsPressed('W'))
	{
		camera->translate({ 0.0f,0.0f,deltaTime });
	}
	if (inputDevice->KeyIsPressed('A'))
	{
		camera->translate({ -deltaTime,0.0f,0.0f });
	}
	if (inputDevice->KeyIsPressed('S'))
	{
		camera->translate({ 0.0f,0.0f,-deltaTime });
	}
	if (inputDevice->KeyIsPressed('D'))
	{
		camera->translate({ deltaTime,0.0f,0.0f });
	}
	if (inputDevice->KeyIsPressed('R'))
	{
		camera->translate({ 0.0f,deltaTime,0.0f });
	}
	if (inputDevice->KeyIsPressed('F'))
	{
		camera->translate({ 0.0f,-deltaTime,0.0f });
	}
}

void SolarSystemGame::drawObjects()
{
	box1->draw();
	box2->draw();
	box3->draw();
	box4->draw();
	box5->draw();
	box6->draw();
	box7->draw();
	box8->draw();
	box9->draw();
	box10->draw();

}
