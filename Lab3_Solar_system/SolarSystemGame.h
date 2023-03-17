#pragma once
#include "Game.h"
#include "BoxObject.h"
#include "PyramidObject.h"
#include "SphereObject.h"

using namespace DirectX::SimpleMath;

class SolarSystemGame : public Game
{
public:
	SolarSystemGame();
	~SolarSystemGame();

	void init() override;
protected:
	void update() override;
	void drawObjects();

private:
	SphereObject* box1;
	SphereObject* box2;
	SphereObject* box3;
	SphereObject* box4;
	SphereObject* box5;
	SphereObject* box6;
	SphereObject* box7;
	SphereObject* box8;
	SphereObject* box9;
	SphereObject* box10;

	//BoxObject* box1;
	//BoxObject* box2;
	//BoxObject* box3;
	//BoxObject* box4;
	//BoxObject* box5;
	//BoxObject* box6;
	//BoxObject* box7;
	//BoxObject* box8;
	//BoxObject* box9;
	//BoxObject* box10;

	PyramidObject* pyramid;
};

