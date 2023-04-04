#pragma once

class Game;

class GameComponent 
{
public:
	virtual void Update(float timeDelta) = 0;
	virtual void Initialize() = 0;
protected:
	Game* game;
};