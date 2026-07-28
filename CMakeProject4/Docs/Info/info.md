# CellGFXの紹介

CellGFXはCPPですべてが記述可能なゲームエンジンです。

「高度で密度が高くシンプル」を目指しています。

使用するにはある程度の高度な知識が必要です。

## 使い方
```cpp
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
	gameObjects["triangle"] = new ExampleCellGameObject();

	CellGFX* gfx = new CellGFX();
	gfx->Run();

	delete gfx;

	for (auto cellData : cellDataMap)
	{
		delete cellData.second;
	}
	return 0;
}
```
基本的にこの宣言のみでゲームが動きます。