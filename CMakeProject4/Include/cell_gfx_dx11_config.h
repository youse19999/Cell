#ifndef CELL_GFX_DX11_COFNIG_H
#define CELL_GFX_DX11_CONFIG_H
#include <d3d11.h>
#include <Windows.h>


const D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
    {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"Nomral", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const char shaderSrc[] = R"(
            
            struct VS_INPUT
            {
                float3 position   : Position;
                float3 normal : Nomral;
                float2 uv : UV;
            };

            struct VS_OUTPUT
            {
                float4 position   : SV_POSITION;
                float3 normal : Normal;
                float2 uv : UV;
            };


            cbuffer ConstantBuffer:register(b0)
            {
                float4x4 camera;
                float4x4 view;
                float4x4 model;
            };

            VS_OUTPUT vs_main(VS_INPUT input)
            {
                float4 pos = float4(input.position, 1);
                pos = mul(pos,camera);
                pos = mul(pos,view);
                pos = mul(pos,model);
                VS_OUTPUT output;
                output.position = pos;
                output.normal = input.normal;
                output.uv = input.uv;
                return output;
            }
            
            float4 ps_main(VS_OUTPUT input) : SV_TARGET
            {
                return float4(input.normal.x, input.normal.y, input.normal.z, 1.0);
            }
        )";

const unsigned int vertexStride = 8;

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