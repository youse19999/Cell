#ifndef CELL_TRANSFORMATION_H
#define CELL_TRANSFORMATION_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <tiny_gltf.h>

//指定のコンスタントバッファー
struct ConstantBuffer
{
	glm::mat4 view;
	glm::mat4 camera;
	glm::mat4 model;
};

//バーテックスの構造体、基本的に横並びになる。
struct Vertex {
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float normal[3] = { 0.0f, 0.0f, 0.0f };
	float uv[2] = { 0.0f, 0.0f };
	int32_t joint[4] = { 0, 0, 0, 0 };
	float weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

//トランスフォーメーションのデータ
struct TransformationData {
	const char* id;
	std::string name;
	bool hasSkin = false;
	//頂点メモリのポインタとしてのID3D11Buffer（COMポインタ）
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	//定数メモリのポインタとしてのID3D11Buffer（COMポインタ）
	std::vector<Vertex> vertex;
	std::vector<unsigned short> index;
	std::vector<std::string> children;
	//テクスチャ
	std::vector<unsigned char> textureBuffer;
	//ここからConstantBuffer分の定数のVRAM
	ConstantBuffer cbuffer;

	unsigned int indexType;

	bool isMesh = false;
};


class CellTransformation {
private:
	TransformationData* data;
public:
	unsigned int Size()
	{
		return data->index.size();
	}
	ID3D11Buffer* GetVertexBuffer()
	{
		return data->vertexBuffer;
	}
	DXGI_FORMAT GetIndexType()
	{
		switch (data->indexType)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			return DXGI_FORMAT_R8_UINT;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			return DXGI_FORMAT_R16_UINT;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			return DXGI_FORMAT_R32_UINT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
	ID3D11Buffer* GetIndexBuffer()
	{
		return data->indexBuffer;
	}
	std::vector<std::string> GetChildren()
	{
		if (data->children.size() == 0)
		{
			std::string s = "?__NOT_FOUNED__?";
			return { s };
		}
		return data->children;
	}
	CellTransformation(TransformationData* data)
	{
		this->data = data;
	}
	void SetData(TransformationData* data)
	{
		this->data = data;
	}
	~CellTransformation()
	{
		std::cout << "release" << std::endl;
		data->vertexBuffer->Release();
		data->indexBuffer->Release();
	}
	void Map(ID3D11Device* device)
	{
		if (data->vertex.size() == 0)
		{
			return;
		}
		D3D11_BUFFER_DESC bufferDesc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		int size = sizeof(Vertex);

		bufferDesc = {};
		bufferDesc.ByteWidth = data->vertex.size()*sizeof(Vertex);
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		

		subresourceData = {};
		subresourceData.pSysMem = data->vertex.data();

		HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, &data->vertexBuffer);

		D3D11_BUFFER_DESC bufferDesc2 = {};
		D3D11_SUBRESOURCE_DATA subresourceData2 = {};

		bufferDesc2 = {};
		bufferDesc2.ByteWidth = data->index.size();
		bufferDesc2.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc2.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subresourceData2 = {};
		subresourceData2.pSysMem = data->index.data();

		hr = device->CreateBuffer(&bufferDesc2, &subresourceData2, &data->indexBuffer);

	}
	std::string GetName()
	{
		return this->data->name;
	}
};

std::unordered_map<std::string, CellTransformation*> vramData;

#endif