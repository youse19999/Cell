#include "CMakeProject4.h"
#include <d3d11.h>

#include <cell_gfx_dx11_config.h>

#define BREAK_ONLY_WINDOW_PROC

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
	SET_UP_CELL_DATA();

	CellGFX* gfx = new CellGFX();
	gfx->Run();

	delete gfx;
	RELEASE_ALL_CELL_DATA();
	return 0;
}
