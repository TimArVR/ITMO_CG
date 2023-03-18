#pragma once
#include "Game.h"
using namespace DirectX::SimpleMath;

class KatamariDamacyGame : public Game
{
public:

	KatamariDamacyGame(LPCWSTR name, int clientWidth, int clientHeight);

	void Initialize();
	void Run();
};
