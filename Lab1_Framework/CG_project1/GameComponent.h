#pragma once
#include ".\Game.h"

class GameComponent
{
public:
	//GameComponent();
	
	virtual void DestroyResources()=0;
	virtual void Draw()=0;
	virtual void Initialize()=0;
	virtual void Reload()=0;
	virtual void Update()=0;

	static Game* gameInstance;	

private:
};

