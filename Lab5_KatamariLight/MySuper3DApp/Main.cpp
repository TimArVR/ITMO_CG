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


	/*ID3D11Debug* debug = 0;
	device->QueryInterface(IID_ID3D11Debug, (void**)&debug);
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/

	game->Run();
	return EXIT_SUCCESS;
}