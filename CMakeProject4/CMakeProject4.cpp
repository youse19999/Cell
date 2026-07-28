#include "CMakeProject4.h"
#include <d3d11.h>
#include <cell_gfx_dx11_config.h>
#include <cell_wit.h>
#include <cell_wit_data.h>
#include <cell_transformation.h>
#include <cell_gameobject.h>
#include<cell_data.h>
#include <cell_gfx_dx11.h>
#include <cell_gfx.h>

//COUT
int main()
{
	//SET_UP_CELL_DATA();

	gameObjects["triangle"] = new ExampleCellGameObject();

	CellGFX* gfx = new CellGFX();
	gfx->Run();

	delete gfx;

	for (auto cellData : cellDataMap)
	{
		delete cellData.second;
	}

	//RELEASE_ALL_CELL_DATA();
	return 0;
}
