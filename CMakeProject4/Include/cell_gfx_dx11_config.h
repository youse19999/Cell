#ifndef CELL_GFX_DX11_COFNIG_H
#define CELL_GFX_DX11_CONFIG_H
#include <d3d11.h>
#include <Windows.h>


const D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
    {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const char shaderSrc[] = R"(
            cbuffer ConstantBuffer:register(b0)
            {
                float4 color;
                float4x4 model;
            };

            float4 vs_main(float3 position : Position) : SV_Position
            {
                float4 pos = float4(position, 1);
                return mul(pos,model);
            }
            
            float4 ps_main() : SV_Target
            {
                return mul(color,model);
            }
        )";

const unsigned int vertexStride = 3;

const std::string CGFX_WINDOW_NAME = "CGFX_WINDOW_NOT_CHANGED";
const std::string CGFX_CLASS_NAME = "CGFX_CLASS_NOT_CHANGED";

const RECT windowRect = RECT{ 0,0,800,600 };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif