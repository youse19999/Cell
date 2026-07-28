#ifndef CELL_GFX_DX11_H
#define CELL_GFX_DX11_H

#define GFX_LOAD_MISSING

#include <vector>
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

template <typename T>
void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

class Trampoline {
private:
	//DirectXのオブジェクト

	std::string name = "HelloWorld";

	std::vector<float> transformationsVertex;
	std::vector<ConstantBuffer> transformationsConstrantBuffer;

	ID3D11Device* device;
	D3D11_VIEWPORT viewport;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* immediateContext;
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexShaderBlob;
	ID3DBlob* pixelShaderBlob;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	//汎用のコンパイラー関数
	void D3D11CompileShader(std::string shader_src, ID3D10Blob** blob, std::string entry, std::string type)
	{
		ID3DBlob* errorBlob = nullptr;
		HRESULT result = D3DCompile(
			shader_src.c_str(),
			shader_src.size(),
			nullptr,
			nullptr,
			nullptr,
			entry.c_str(),
			type.c_str(),
			0,
			0,
			blob,
			&errorBlob);
		if (FAILED(result))
		{
			std::cout << (char*)errorBlob->GetBufferPointer() << std::endl;
			return;
		}
	}
	//単一のリリース関数
	void Release()
	{
		SafeRelease(inputLayout);
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(vertexShaderBlob);
		SafeRelease(pixelShaderBlob);
		SafeRelease(renderTargetView);
		SafeRelease(backBuffer);
		SafeRelease(swapChain);

		if (immediateContext)
		{
			immediateContext->ClearState();
			SafeRelease(immediateContext);
		}
		SafeRelease(device);
	}
protected:
	//トランスフォーメーションの実行
	void AddTransformation(TransformationData transformation)
	{
		for (auto vertex : transformation.vertex) {
			transformationsVertex.push_back(vertex);
		}
		transformationsConstrantBuffer.push_back(transformation.cbuffer);
	}
	//頂点シェダー向けの情報の設定
	void BindData()
	{
		for (auto data : vramData) {
			data.second->Map(device);
			//AddTransformation(CellTransformation::Transformation(gameObject.second,gameObject.second->WhatIsThis()->GetVertexBuffer(), gameObject.second->WhatIsThis()->GetColor()));
		}
	}
	//ゲームオブジェクトの更新
	void UpdateGameObject()
	{
		for (auto object : gameObjects) {
			object.second->UpdateConstantBuffer(immediateContext);
			object.second->Update();
		}
	}
	void InitializeGameObject()
	{
		for (auto object : gameObjects) {
			object.second->CreateConstantBuffer(device);
		}
	}
	void RenderObject(CellGameObject* object)
	{
		UINT stride = vertexStride * sizeof(float);
		UINT offset = 0;
		if (!object->GetVramData())
		{
			return;
		}
		ID3D11Buffer* vertexBuffer = object->GetVramData()->GetVertexBuffer();
		immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		
		immediateContext->IASetIndexBuffer(object->GetVramData()->GetIndexBuffer(), object->GetVramData()->GetIndexType(), 0);
		immediateContext->IASetInputLayout(inputLayout);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* constantBuffer = object->GetConstantBuffer();
		immediateContext->VSSetConstantBuffers(0, 1, &constantBuffer);
		immediateContext->PSSetConstantBuffers(0, 1, &constantBuffer);

		immediateContext->VSSetShader(vertexShader, nullptr, 0);
		immediateContext->PSSetShader(pixelShader, nullptr, 0);


		immediateContext->RSSetViewports(1, &viewport);

		immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

		immediateContext->DrawIndexed(object->GetVramData()->Size(), 0, 0);
	}

	//レンダー
	void Render()
	{
		MSG msg = {};
		bool closed = false;
		while (!closed) {
			UpdateGameObject();
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					closed = true;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			immediateContext->ClearRenderTargetView(renderTargetView, clear_color);
			for (auto obj : gameObjects) {
				RenderObject(obj.second);
			}

			swapChain->Present(1, 0);
		}
		Release();
	}
	//全パイプライン初期化の抽象化
	void Initialize()
	{
		//HINSTANCEの取得
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

		//ウィンドウクラスの取得
		WNDCLASS wc = { };

		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = CGFX_CLASS_NAME.data();

		RegisterClass(&wc);

		//ウィンドウの作成
		HWND hwnd = CreateWindowEx(
			0,
			CGFX_CLASS_NAME.data(),
			CGFX_WINDOW_NAME.data(),
			WS_OVERLAPPEDWINDOW,

			windowRect.left, windowRect.top, windowRect.right, windowRect.bottom,

			NULL,
			NULL,
			hInstance,
			NULL
		);

		//ウィンドウの作成に失敗したら
		if (hwnd == NULL)
		{
			return;
		}

		ShowWindow(hwnd, 1);


		//ウィンドウの矩形を取得
		RECT rect;
		GetClientRect(hwnd, &rect);
		int window_width = rect.right - rect.left;
		int window_height = rect.bottom - rect.top;

		//スワップチェーンのバッファ用ビューポートの作成
		viewport.Width = (float)window_width;
		viewport.Height = (float)window_height;

		//スワップチェーンのバッファ用ビューポートの作成

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = window_width;
		sd.BufferDesc.Height = window_height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;


		D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

		HRESULT hr = S_OK;
		D3D_FEATURE_LEVEL FeatureLevel;

		hr = D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			&FeatureLevels,
			1,
			D3D11_SDK_VERSION,
			&sd,
			&swapChain,
			&device,
			&FeatureLevel,
			&immediateContext);

		if (hr) {
			return;
		}


		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		if (hr) {
			return;
		}
		hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		if (hr) {
			return;
		}

		if (hr) {
			return;
		}

		D3D11CompileShader(shaderSrc, &vertexShaderBlob, "vs_main", "vs_5_0");
		D3D11CompileShader(shaderSrc, &pixelShaderBlob, "ps_main", "ps_5_0");

		HRESULT result = device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			nullptr,
			&vertexShader);
		if (FAILED(result))
		{
			return;
		}
		result = device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(),
			nullptr,
			&pixelShader);
		if (FAILED(result))
		{
			return;
		}

		{

			HRESULT result = device->CreateInputLayout(
				input_element_desc,
				ARRAYSIZE(input_element_desc),
				vertexShaderBlob->GetBufferPointer(),
				vertexShaderBlob->GetBufferSize(), &inputLayout);
			if (FAILED(result))
			{
				return;
			}
		}


		viewport = {};
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			int window_width = rect.right - rect.left;
			int window_height = rect.bottom - rect.top;
			viewport.Width = (float)window_width;
			viewport.Height = (float)window_height;
		}
	}
	void CreateBuffer()
	{
		InitializeGameObject();
	}
};

#endif