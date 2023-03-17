#include <iostream>
#include "KatamariDamacyGame.h"
#include "Transform.h"
#include "SimpleMath.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main()
{
	//Assimp::Importer importer;
	KatamariDamacyGame* game = new KatamariDamacyGame(L"KatamariGame ", 800, 800);
	game->Run();
	return EXIT_SUCCESS;
}