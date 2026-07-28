# CellGFXの特徴

CellGFXは**グラフィックライブラリ**です。

UnityやUEなどのようなエンジンではないです。

このエンジンは、ヘッダーオンリーです。

ヘッダーの読み込みの順序で様々な変化が起こる魔法のようなものです。

基本的に、ヘッダー側の挙動の変更が前提なので難易度の高いライブラリにはなってます。

```cpp
#include <cell_gfx_dx11.h>
#include <cell_gfx.h>
```
と読み込めば、directx11で動き
```cpp
#include <cell_gfx_vulkan.h>
#include <cell_gfx.h>
```
と読み込めば、vulkanで動きます。


```cpp
#include <cell_gfx_dx11.h>
#include <cell_gfx.h>

int main()
{
	Cell cell;
	cell.Run();
}
```
のみで初期化が完了します。
```cpp
#include <cell_gfx_dx11.h>

#define USE_Z_BUFFER

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define USE_3FLOAT_3FLOAT_2FLOAT_VERTEX_INPUT

#define VERTEX_MEMORY_SIZE 4096

#define TEXTURE_MEMORY_COUNT 4

#include <cell_gfx.h>

int main()
{

	Cell cell;
	cell.Run();
}
```
のように書くこと以外は許可されていません。
ゲームのロジックは、CellGameObjectクラス内にすべて書きます。

```cpp
#include <cell_game_object.h>

class Triangle::CellGameObject{
{
	WIT* wit = WIT::GET_WIT_ID(0);
}
```