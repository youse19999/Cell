#ifndef CELL_TRANSFORMATION_H
#define CELL_TRANSFORMATION_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

//指定のコンスタントバッファー
struct ConstantBuffer
{
	glm::vec4 color;
	glm::mat4 model;
};

//トランスフォーメーションのデータ
struct TransformationData {
	const char* id;
	//頂点メモリのポインタとしてのID3D11Buffer（COMポインタ）
	ID3D11Buffer* vertexBuffer;
	//定数メモリのポインタとしてのID3D11Buffer（COMポインタ）
	//ID3D11Buffer* constantBuffer;
	//ID3D11Bufferが無効ならオフセットがここからvertexのサイズを頂点VRAM
	std::vector<glm::vec3> vertex;
	//ここからConstantBuffer分の定数のVRAM
	ConstantBuffer cbuffer;
};


class CellTransformation {
private:
	TransformationData* data;
public:
	unsigned int Size()
	{
		return data->vertex.size();
	}
	ID3D11Buffer* GetVertexBuffer()
	{
		return data->vertexBuffer;
	}
	CellTransformation(std::string id,std::vector<glm::vec3> _vertex, glm::vec4 color)
	{
		data = new TransformationData();
		std::swap(data->vertex, _vertex);
	}
	void Map(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(glm::vec3) * data->vertex.size();
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.StructureByteStride = vertexStride * sizeof(float);

		subresourceData = {};
		subresourceData.pSysMem = data->vertex.data();

		HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, &data->vertexBuffer);
	}
};

std::unordered_map<const char*, CellTransformation*> vramData;

#endif